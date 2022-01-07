import React from 'react';
import Container from '@mui/material/Container';
import { Box } from '@mui/system';
import Slider from '@mui/material/Slider';
import { Hook } from "../Hook";
import FormGroup from '@mui/material/FormGroup';
import FormControlLabel from '@mui/material/FormControlLabel';
import Switch from '@mui/material/Switch';
import { Button } from '@mui/material';
import { Socket } from 'socket.io-client';
import socketIOClient from "socket.io-client";
const url = "http://127.0.0.1:8090";
const socket = socketIOClient(url);
var timeout;
var logged=localStorage.getItem('logged');

export default function Notes() {
  if (logged==0) window.location.href = "./";
  const [name, setName] = Hook("name", "");
  function preventHorizontalKeyboardNavigation(event) {
    if (event.key === 'ArrowLeft' || event.key === 'ArrowRight') {
      event.preventDefault();
    }
  }

  function valuetext(value) {
    return `${value}°`;
  }


  const handleChangeH = (event, newValue) => {
    console.log('H'+newValue);
    socket.send('H'+newValue)
  };

  const handleChangeV = (event, newValue) => {
    console.log('V'+newValue);
    socket.send('V'+newValue);
  };
  const handleChangeT = (event, newValue) => {
    if(newValue==true){
    socket.send('MODE1');
    console.log('MODE1');
    }else if(newValue==false){
      socket.send('MODE0');
      console.log('MODE0');
    }
  };

  const { value } = 50;

  return (
    <div>
      <Container maxWidth="sm">
      <FormGroup>
      <FormControlLabel control={<Switch onChange={handleChangeT}/>} label="Manual control ON/OFF" />
    </FormGroup>
      Angle of the panel vertically
      <Box sx={{ height: 300}}>     
      
      <Slider
        sx={{
          '& input[type="range"]': {
            WebkitAppearance: 'slider-vertical',
          },
        }}
        orientation="vertical"
        defaultValue={50}
        onKeyDown={preventHorizontalKeyboardNavigation}
        valueLabelDisplay="auto"
        step={1}
        min={30}
        max={150}
        onChangeCommitted	={handleChangeH}
      />
      
      </Box>
      </Container>
      <Container maxWidth="sm">
      Angle of the panel horizontally
      <Box sx={{ width: 600}}> 
      <Slider
        aria-label="Temperature"
        getAriaValueText={valuetext}
        valueLabelDisplay="auto"
        step={1}
        min={30}
        max={150}
        defaultValue={100}
        value={name}
        onChange={(e) => setName(e.target.value)}
        onChangeCommitted	={handleChangeV}
      />
      </Box>
      </Container>
      
    </div>
  )
}
