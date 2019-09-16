/* Untyped file generated by genType. */

const Curry = require('bs-platform/lib/js/curry.js');

const JsExportBS = require('./JsExport.bs');

const JsResult_fromResult = function (Arg1) {
  const result = JsExportBS.JsResult[0](Arg1);
  return {data:result[0], error:result[1]}
};;
exports.JsResult_fromResult = JsResult_fromResult

const toMeasurementValue = function (Arg1) {
  const result = JsExportBS.toMeasurementValue(Arg1);
  return {data:result[0], error:result[1]}
};;
exports.toMeasurementValue = toMeasurementValue

const toPredictionResolutionGroup = function (Arg1, Arg2, Arg3) {
  const result = Curry._3(JsExportBS.toPredictionResolutionGroup, Arg1, (Arg2 == null ? undefined : Arg2), Arg3);
  return result
};;
exports.toPredictionResolutionGroup = toPredictionResolutionGroup

const score = function (Arg1, Arg2, Arg3) {
  const result = Curry._3(JsExportBS.score, Arg1, (Arg2 == null ? undefined : Arg2), Arg3);
  return result
};;
exports.score = score

const toM = function (Arg1) {
  const result = JsExportBS.toM([Arg1.measurement, Arg1.time]);
  return result
};;
exports.toM = toM

const itemArray = function (Arg1) {
  const result = JsExportBS.itemArray(Arg1.map(function _element(ArrayItem) { return [ArrayItem.measurement, ArrayItem.time]}));
  return result
};;
exports.itemArray = itemArray

const getScoreOverTime = function (Arg1, Arg2, Arg3) {
  const result = Curry._3(JsExportBS.getScoreOverTime, Arg1.map(function _element(ArrayItem) { return [ArrayItem.measurement, ArrayItem.time]}), Arg2.map(function _element(ArrayItem1) { return [ArrayItem1.measurement, ArrayItem1.time]}), [Arg3.measurement, Arg3.time]);
  return {data:result[0], error:result[1]}
};;
exports.getScoreOverTime = getScoreOverTime
