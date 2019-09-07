type time = int;
type ts = array(time);

// let agentPredictions = [5, 6, 7];
// let marketPredictions = [1, 2, 3, 7, 8];
// let perimeter = [5, 8];
// let points = [5, 6, 7, 8];

module SortedArray = {
  type t('a) = array('a);

  let fromArray = (a: array('a)) =>
    a |> Belt.SortArray.stableSortBy(_, compare);

  let uniq = (a: array('a)) =>
    a |> Array.to_list |> List.sort_uniq(compare) |> Array.of_list;

  let make = (t: array('a)) =>
    Belt.SortArray.isSorted(t, compare) ? Some(t) : None;

  let concat = (a: t('a), b: t('a)): t('a) =>
    Belt.Array.concat(a, b) |> fromArray;

  let filterOutLessThan = (lowestAllowedValue: 'a, t: t('a)): t('a) =>
    Belt.Array.keep(t, x => x >= lowestAllowedValue);

  let filterOutGreaterThan = (highestAllowedValue: 'a, t: t('a)): t('a) =>
    Belt.Array.keep(t, x => x >= highestAllowedValue);

  let firstBeforeValue = (v: 'a, t: t('a)) => {
    let firstIndex = Belt.Array.getIndexBy(t, r => r >= v);
    let previousIndex =
      switch (firstIndex) {
      | None => None
      | Some(0) => None
      | Some(n) => Some(n)
      };
    let previousValue =
      previousIndex |> Belt.Option.flatMap(_, Belt.Array.get(t));
    previousValue;
  };

  let firstAtOrBeforeValue = (v: 'a, t: t('a)) => {
    let firstIndex = Belt.Array.getIndexBy(t, r => r >= v);
    let previousIndex =
      switch (firstIndex) {
      | None => None
      | Some(0) => None
      | Some(n) => Some(n)
      };
    let previousValue =
      previousIndex |> Belt.Option.flatMap(_, Belt.Array.get(t));
    previousValue;
  };

  let filterOutLessThanButKeepFirstBefore =
      (lowestAllowedValue: 'a, t: t('a)): t('a) => {
    let atAndAfter = filterOutLessThan(lowestAllowedValue, t);
    let previousValue = firstBeforeValue(lowestAllowedValue, t);
    switch (previousValue) {
    | None => atAndAfter
    | Some(e) => concat([|e|], atAndAfter) |> fromArray
    };
  };

  let min = Belt.Array.get(_, 0);
  let max = t => Belt.Array.get(t, Belt.Array.length(t));
};

module StartAtArray = {
  type t('a) = array('a);

  let recentValueToPoint = (v, t: t('a)) =>
    Belt.Array.getIndexBy(_, r => r >= v);
};

module StartAtDistribution = {
  type t('a, 'b) = {
    finalX: 'a,
    pointXY: array(('a, 'b)),
  };

  //The Xs should be unique and should be sorted.
  let make = (~pointXY, ~finalX) => {finalX, pointXY};

  let map = (fn, t: t('a, 'b)) => {
    finalX: t.finalX,
    pointXY: t.pointXY |> Array.map(((x, y)) => (x, fn(y))),
  };

  let xs = (t: t('a, 'b)) => t.pointXY |> Array.map(((x, _)) => x);
  let ys = (t: t('a, 'b)) => t.pointXY |> Array.map((_, y) => y);
  let minX = (t: t('a, 'b)) => SortedArray.min(t |> xs);

  // Finds relevant (x,y) point at or before a specific x point.
  let xPointToPointXY = (xPoint, t: t('a, 'b)) =>
    xPoint > t.finalX
      ? None
      : t.pointXY
        |> Belt.Array.reverse
        |> Belt.Array.getBy(_, ((x, _)) => x <= xPoint);

  let xPointToPointX = (xPoint, t: t('a, 'b)) =>
    xPointToPointXY(xPoint, t) |> Belt.Option.map(_, ((x, _)) => x);

  let xPointToPointY = (xPoint, t: t('a, 'b)) =>
    xPointToPointXY(xPoint, t) |> Belt.Option.map(_, ((_, y)) => y);

  let toSortedArray = (t: t('a, 'b)) => {
    switch (xPointToPointXY(t.finalX, t)) {
    | Some((x, y)) when x == t.finalX => Some(t.pointXY)
    | Some((x, y)) => Some(SortedArray.concat(t.pointXY, [|(x, y)|]))
    | _ => None
    };
  };

  let integral = (t: t(float, float)) => {
    switch (t |> toSortedArray |> Belt.Option.map(_, Belt.Array.length)) {
    | Some(0) => 0.
    | Some(1) => 0.
    | None => 0.
    | Some(n) =>
      Belt.Array.makeBy(n - 1, r => r)
      |> Belt.Array.map(
           _,
           r => {
             let (lastX, lastY) = t.pointXY |> Belt.Array.getUnsafe(_, r);
             let (nextX, _) = t.pointXY |> Belt.Array.getUnsafe(_, r + 1);
             (nextX -. lastX) *. lastY;
           },
         )
      |> Belt.Array.reduce(_, 0., (a, b) => a +. b)
    };
  };

  let compareXs = ((x1, _), (x2, _)) => compare(x1, x2);
  let sortXUniq = t =>
    t.pointXY |> Array.to_list |> List.sort_uniq(compareXs) |> Array.of_list;

  // If ether is empty, should return empty list.
  let relevantXs = (a: t('a, 'b), b: t('a, 'c)) => {
    let firstCommonMin =
      switch (minX(a), minX(b)) {
      | (Some(a), Some(b)) => Some(min(a, b))
      | _ => None
      };
    switch (firstCommonMin) {
    | Some(firstCommonMin) =>
      SortedArray.concat(xs(a), xs(b))
      |> SortedArray.filterOutLessThan(firstCommonMin)
    | None => [||]
    };
  };

  let product = (t1: t('a, 'b), t2: t('a, 'c)): t('a, ('b, 'c)) => {
    let xs = relevantXs(t1, t2);
    {
      finalX: min(t1.finalX, t2.finalX),
      pointXY:
        xs
        |> Array.map(x =>
             switch (xPointToPointY(x, t1), xPointToPointY(x, t2)) {
             | (Some(y1), Some(y2)) => Some((x, (y1, y2)))
             | _ => None
             }
           )
        |> Utility.Array.concatSome,
    };
  };
};