import React from "react";
import styled from "styled-components";
import colors from "./colors";

const Button = styled.div`
background-color: ${colors.accent};
color: ${colors.main};
display: flex;
justify-content: center;
align-items: center;
width: ${props => props.width};
height: ${props => props.height};
font-weight: bold;
cursor: pointer;
`
export default Button;