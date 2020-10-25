import React, {useEffect, useRef} from 'react';
import styled from "styled-components";
import colors from "./colors.js";
import logo from './logo.svg';
import Button from "./Button";
import EditBox from "./EditBox";
import './App.css';
import {gsap} from 'gsap';
import AnswerScreen from "./AnswScreen";

const Decimal = require('decimal.js')

Decimal.set({precision: 100, rounding: 3})

const AppWrapper = styled.div`
background-color: ${colors.main};
width: 100vw;
height: 100vh;
z-index: 0;
display: flex;
flex-flow: column;
justify-content: center;
align-items: center;
color: ${colors.text};
position: relative;
`

const InputWrapper = styled.div`
width: 75vw;
height: 30vh;
display: flex;
justify-content: center;
align-items: center;
`;

const BoxWrapper = styled.div`
width: 50%;
height: 50%;
display: flex;
flex-flow: column;
justify-content: center;
align-items: center;
`;

const PointTitle = styled.span`
margin: 2vh;
font-size: x-large;
font-weight: bold;
`;

const ThirdPoint = styled(InputWrapper)`
margin-bottom: 2vh;
`

const checkLength = (arr) => {
    let ans = true;
    arr.forEach((input) => {ans &= (input.value.length > 0)})
    return ans;
}

const checkValidText = (arr) => {
    let ans = true;
    arr.forEach((input) => {ans &= (/^\d*\.?\d+$/.test(input.value))})
    return ans;
}

function App() {
    const btn = useRef(null);
    const answPg = useRef(null);

    useEffect(() => {
        gsap.to(answPg.current, {autoAlpha: 0, duration: 0})
    }, [answPg])

    const calculate = () => {
        gsap.fromTo(btn.current, {backgroundColor: "#f3e2bc"}, {backgroundColor: "#fab40a", duration: 1.5})

        const inputs = Array.from(document.getElementsByTagName("input"));

        if(checkLength(inputs) && checkValidText(inputs))
        {
            const fp = [new Decimal(inputs[0].value), new Decimal(inputs[1].value)];
            const sp = [new Decimal(inputs[2].value), new Decimal(inputs[3].value)];
            const tp = [new Decimal(inputs[4].value), new Decimal(inputs[5].value)];

            const detValue = fp[0].times(sp[1]).plus(  sp[0].times(tp[1])).plus( tp[0].times(fp[1])).minus( tp[0].times(sp[1])).minus( fp[0].times(tp[1])).minus( sp[0].times(fp[1]));
            const vcValue = ((sp[0].minus(fp[0])).times((tp[1].minus(fp[1])))).minus(((tp[0].minus(fp[0])).times(sp[1].minus(fp[1]))));
            console.log(detValue.toPrecision(50));
            console.log(vcValue.toPrecision(50));
            console.log((vcValue.minus(detValue)).toPrecision(50));

            const ans = document.getElementById("answer");
            if(vcValue.eq(0))
                ans.innerText = "SAME"
            else
                ans.innerText = (vcValue > 0) ? "RIGHT" : "LEFT";



            const canvas = answPg.current.children[0];
            const ctx = canvas.getContext('2d');
            let maxX = 0, maxY = 0;
            inputs.forEach((inputs) => {
                if(inputs.value % 2 === 0)
                    maxX = Decimal.max(maxX, inputs.value);
                else
                    maxY = Decimal.max(maxY, inputs.value);
            })


            const cMidX = canvas.width / 2;
            const cMidY = canvas.height / 2;
            const scale = Decimal.min(canvas.width / maxX, canvas.height / maxY);

            ctx.strokeStyle = colors.accent
            ctx.lineWidth = 1;



            if(scale < 1)
            {
                const sfp = [fp[0].times(scale), fp[1].times(scale)];
                const ssp = [sp[0].times(scale), sp[1].times(scale)];

                const mid = [(sfp[0].plus(ssp[0])).div(2), (sfp[1].plus(ssp[1])).div(2)]
                ctx.translate((mid[0] > cMidX) ? -(mid[0] - cMidX) : (cMidX - mid[0]),
                    (mid[1] > cMidY) ? -(mid[1] - cMidY) : (cMidY) - mid[1])

                ctx.beginPath();
                ctx.moveTo(sfp[0], sfp[1]);
                ctx.fillText('A',sfp[0], sfp[1], 5);
                ctx.lineTo(ssp[0], ssp[1]);
                ctx.fillText('B',ssp[0], ssp[1], 5);
                ctx.stroke();
                ctx.closePath();
                ctx.strokeRect(tp[0], tp[1], 1, 1);
            } else
            {
                const mid = [(fp[0].plus(sp[0])).div(2), (fp[1].plus(sp[1])).div(2)]
                ctx.translate((mid[0] > cMidX) ? -(mid[0] - cMidX) : (cMidX - mid[0]),
                    (mid[1] > cMidY) ? -(mid[1] - cMidY) : (cMidY) - mid[1])

                ctx.beginPath();
                ctx.moveTo(fp[0], fp[1]);

                ctx.fillText('A', fp[0], fp[1], 5);
                ctx.lineTo(sp[0], sp[1]);
                ctx.stroke();

                ctx.fillText('B', sp[0], sp[1], 5);
                ctx.closePath();
                ctx.strokeRect(tp[0], tp[1], 1, 1);
            }

            gsap.fromTo(answPg.current,{autoAlpha: 1, translateY: "-100vh"}, {autoAlpha: 1, translateY: 0, duration: 1})
        } else {
            alert("All fields must be filled with numbers only")
        }
    }

  return (
    <AppWrapper>
        <input id={"test"} type={"file"} />
        <AnswerScreen ref={answPg}>

        </AnswerScreen>
        <InputWrapper>
            <BoxWrapper>
                <PointTitle>
                    Enter first point
                </PointTitle>
                <span>
                    X position
                </span>
                <EditBox />
                <span>
                    Y position
                </span>
                <EditBox />
            </BoxWrapper>
            <BoxWrapper>
                <PointTitle>
                    Enter second point
                </PointTitle>
                <span>
                    X position
                </span>
                <EditBox />
                <span>
                    Y position
                </span>
                <EditBox />
            </BoxWrapper>
        </InputWrapper>
        <ThirdPoint>
            <BoxWrapper>
                <PointTitle>
                    Enter a point to check
                </PointTitle>
                <span>
                    X position
                </span>
                <EditBox />
                <span>
                    Y position
                </span>
                <EditBox />
            </BoxWrapper>
        </ThirdPoint>
        <Button ref={btn} onClick={calculate} height={"10vh"} width={"25vw"}>
            Click me!
        </Button>
    </AppWrapper>
    );
}

export default App;
