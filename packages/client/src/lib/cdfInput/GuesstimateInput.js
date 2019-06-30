import React from "react";
import {Guesstimator} from '../guesstimator/index';
import {Input} from "antd";
import {Samples} from "@foretold/cdf";
import _ from "lodash"

const minMaxRatio = (ratio) => {
    if (ratio < 100000){
        return "SMALL"
    } else if (ratio < 10000000){
        return "MEDIUM"
    } else {
        return "LARGE"
    }
};

const toCdf = (values) => {
    const samples = new Samples(values);
    let min = _.min(values);
    let max = _.max(values);
    let ratioSize = minMaxRatio(max/min);
    let width = ratioSize == "SMALL" ? 20 : 1;
    const cdf = samples.toCdf({size:1000, width});
    return [cdf.ys, cdf.xs, ratioSize == "LARGE"];
};

export class GuesstimateInput extends React.Component {
    constructor(props) {
        super(props);
        this.state = {value: '', items:[]};
        this.handleChange = this.handleChange.bind(this);
        this.textInput = React.createRef();
    }

    componentDidMount(){
        if (this.props.focusOnRender){
            this.textInput.focus();
        }
    }

    handleChange(event) {
        const text = event.target.value;

        let [error, item] = Guesstimator.parse({text});
        let parsedInput = item.parsedInput;
        let what = new Guesstimator({parsedInput: parsedInput});
        let foo = what.sample(this.props.sampleCount);
        let values = foo.values;

        if (!!values) {
            this.setState({value: event.target.value, items: values});
        } else {
            this.setState({value: event.target.value, items: []});
        }

        this.props.onUpdate(!!values ? toCdf(values): [[], [], false]);
        this.props.onChange(text);
      }
    
    render() {
        return (<Input type="text" value={this.state.value} onChange={this.handleChange} ref={input => this.textInput = input}/>
        )
    }
}