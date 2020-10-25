import React from "react";
import styled from "styled-components"
import colors from "./colors";

const EditWrapper = styled.input`
border: ${colors.accent} 2px solid;
background-color: ${colors.main};
height: ${props => props.height};
width: ${props => props.width};
margin: 2vh;
color: ${colors.accent};
`

const EditBox = (props) => {

    return (
        <EditWrapper width={props.width} height={props.height} type={"text"} min={0} max={100}>
            {props.text}
        </EditWrapper>
    )
}

export default EditBox;