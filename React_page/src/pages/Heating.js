import React from 'react';
import Typography from '@mui/material/Typography';
import Button from '@mui/material/Button';
import Container from '@mui/material/Container';
import { Stack } from '@mui/material';
import { Divider } from '@mui/material';
import { Box } from '@mui/system';
import { grid } from '@mui/system';
import CircularSlider from '@fseehawer/react-circular-slider';
import { useState, useEffect } from 'react';
import socketIOClient from "socket.io-client";
import NavBar from "./NavBar";
var timeout;
var timeout1;
var timeout2;
const url = "http://127.0.0.1:8090";
const socket = socketIOClient(url);
var temK=localStorage.getItem('temK');
var temA=localStorage.getItem('temA');
var temE=localStorage.getItem('temE');
var check=16;
var logged=localStorage.getItem('logged');

export default function Create() {

  if (logged==0) window.location.href = "./";
const handleChangeK = (newValue) => {
  timeout && clearTimeout(timeout);
  timeout = setTimeout(() => {
  console.log("K"+newValue);
  socket.send("K"+newValue);

  
}, 1000);

for (let i = 0; i < 25; i++) {
  if (newValue==((i*0.5)+16)) temK=i;
  }

localStorage.setItem('temK', temK);
return temK;
};


const handleChangeA = (newValue) => {
  timeout1 && clearTimeout(timeout1);
  timeout1 = setTimeout(() => {
  console.log("A"+newValue);
  socket.send("A"+newValue);

  
}, 1000);

for (let i = 0; i < 25; i++) {
  if (newValue==((i*0.5)+16)) temA=i;
  }

localStorage.setItem('temA', temA);
return temA;
};


const handleChangeE = (newValue) => {
  timeout2 && clearTimeout(timeout2);
  timeout2 = setTimeout(() => {
  console.log("E"+newValue);
  socket.send("E"+newValue);

  
}, 1000);

for (let i = 0; i < 25; i++) {
  if (newValue==((i*0.5)+16)) temE=i;
  }

localStorage.setItem('temE', temE);
return temE;
};


  return (
    <div>


      <Container>
      
  <Stack direction="row"
  justifyContent="center"
  alignItems="center"
  spacing={8}>
      <Box component="span" sx={{ p: 2, border: 'solid grey',  gap: 3 }}>
        <Stack
          direction="column-reverse"
          justifyContent="center"
          alignItems="center"
          spacing={2}
          >
          
          <Button
          onClick={() => socket.send('ON003')}
          variant="contained">
          ON/OFF</Button>
          Manual heating in bedroom
          <CircularSlider
          label="Set temperature in bedroom"
          width={250}
          min={16}
          max={28}
          labelColor="#66c2ff"
          progressColorFrom="#80ccff"
          progressColorTo="#006bb3"
          dataIndex={temE}
          data={["16.0" , "16.5" , "17.0" , "17.5" , "18.0", "18.5" , "19.0" , "19.5", "20.0" , "20.5" , "21.0" , "21.5" , "22.0" , "22.5" , "23.0" , "23.5" , "24.0" , "24.5" , "25.0" , "25.5" , "26.0" , "26.5" , "27.0" , "27.5" , "28.0"]}
          knobPosition="bottom"
          appendToValue="°C"
          labelBottom={true}
          onChange={handleChangeE}
        />
        
        </Stack>
      </Box>

<Box component="span" sx={{ p: 2, border: 'solid grey',  gap: 3 }}>
        <Stack
          direction="column-reverse"
          justifyContent="center"
          alignItems="center"
          spacing={2}
          >

          <Button
          onClick={() => socket.send('ON001')}
          variant="contained">
          ON/OFF</Button>
          Manual heating in bathroom
          <CircularSlider
          label="Set temperature in bathroom"
          width={250}
          min={16}
          max={28}
          labelColor="#66c2ff"
          data={["16.0" , "16.5" , "17.0" , "17.5" , "18.0", "18.5" , "19.0" , "19.5", "20.0" , "20.5" , "21.0" , "21.5" , "22.0" , "22.5" , "23.0" , "23.5" , "24.0" , "24.5" , "25.0" , "25.5" , "26.0" , "26.5" , "27.0" , "27.5" , "28.0"]}
          dataIndex={temA}
          knobPosition="bottom"
          appendToValue="°C"
          labelBottom={true}
          onChange={handleChangeA}
        />
        </Stack>
      </Box>
      <Box component="span" sx={{ p: 2, border: 'solid grey',  gap: 3 }}>
        <Stack
          direction="column-reverse"
          justifyContent="center"
          alignItems="center"
          spacing={2}
          >

          <Button
          onClick={() => socket.send('ON002')}
          variant="contained">
          ON/OFF</Button>
          Manual heating in kitchen
          <CircularSlider
          label="Set temperature in kitchen"
          width={250}
          min={16}
          max={28}
          dataIndex={temK}
          labelColor="#66c2ff"
          data={["16.0" , "16.5" , "17.0" , "17.5" , "18.0", "18.5" , "19.0" , "19.5", "20.0" , "20.5" , "21.0" , "21.5" , "22.0" , "22.5" , "23.0" , "23.5" , "24.0" , "24.5" , "25.0" , "25.5" , "26.0" , "26.5" , "27.0" , "27.5" , "28.0"]}
          knobPosition="bottom"
          appendToValue="°C"
          labelBottom={true}
          onChange={handleChangeK}
        />
        </Stack>
      </Box> 
           

      
      </Stack>
      </Container>
    
    
    </div>
  )
}
