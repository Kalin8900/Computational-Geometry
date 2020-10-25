import React from "react";
import styled from 'styled-components'
import colors from "./colors";

const Wrapper = styled.div`
width: 75vw;
height: 75vh;
background-color: ${colors.second};
position: absolute;
top: 12.5vh;
left: 12.5vh;
display: flex;
flex-flow: column;
//visibility: hidden;
`;

const StyledCanvas = styled.canvas`
width: 95%;
margin: 2.5%;
height: 85%;
border: 2px ${colors.accent} solid
`

const AnswerText = styled.span`
display: flex;
justify-content: center;
align-items: center;
font-weight: bold;
font-size: x-large;
`
const Answer = styled.span`
color: ${colors.accent};
margin: 0 5px
`

const AnswerScreen = React.forwardRef((props, ref) => (
        <Wrapper ref={ref}>
            <StyledCanvas>

            </StyledCanvas>
            <AnswerText>
                This point is on the <Answer id={"answer"}> none </Answer> side
            </AnswerText>
        </Wrapper>
))

export default AnswerScreen;