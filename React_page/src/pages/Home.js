import React from 'react';
import Container from '@mui/material/Container';
import { Stack } from '@mui/material';
import { Box } from '@mui/system';
import House from './pic.png';
import PopsKitchen from './dialogs_kitchen';
import PopsBedroom from'./dialogs_bedroom';
import PopsBathroom from'./dialogs_bathroom';
import socketIOClient from "socket.io-client";
import { Button } from '@mui/material';
import { useEffect } from 'react';
import { Typography } from '@mui/material';
import NavbarCollapse from "react-bootstrap/esm/NavbarCollapse";
import { Navbar } from "react-bootstrap";
import { Nav } from "react-bootstrap";
import { NavDropdown } from "react-bootstrap"
var logged=localStorage.getItem('logged');


const url = "http://127.0.0.1:8090";
const socket = socketIOClient(url);
var msg1=localStorage.getItem('temCurrA');
var msg2=localStorage.getItem('temCurrK');
var msg3=localStorage.getItem('temCurrE');

export default function Home() {
    if (logged==0) window.location.href = "./";

    return (
        <Container>
             <Stack
          direction="column-reverse"
          justifyContent="center"
          alignItems="center"
          spacing={2}
          >
            <Box id='housePlan'
      sx={{
        marginTop: '-10px',
        marginBottom:'10px',
        width: 900,
        height: 468,
        backgroundImage: `url(${House})`,
      }}
    >

<Box 
    sx={{
        width: 200,
        height: 100,
        position: 'relative',
        bottom: '-140px',
        right: '-650px'
    }}
>       <Stack
          direction="column"
          justifyContent="center"
          alignItems="center"
          spacing={0}
          >
        <PopsKitchen>
        
        </PopsKitchen>  
        <Typography variant="h6" component="h2">
        <div id="temCurrK">{msg2}°C</div> 
        </Typography>
        </Stack>
    </Box>  

    <Box 
    sx={{
        width: 200,
        height: 100,
        position: 'relative',
        bottom: '-40px',
        right: '-100px'
    }}
>       <Stack
          direction="column"
          justifyContent="center"
          alignItems="center"
          spacing={0}
          >
        <PopsBedroom>
        
        </PopsBedroom>   
        <Typography variant="h6" component="h2">
        <div>{msg3}°C</div>
        </Typography>
        </Stack>
    </Box>

    <Box 
    sx={{
        width: 200,
        height: 100,
        position: 'relative',
        bottom: '-40px',
        right: '-425px'
    }}
>       <Stack
          direction="column"
          justifyContent="center"
          alignItems="center"
          spacing={0}
          >
        <PopsBathroom>
        
        </PopsBathroom>   
        <Typography variant="h6" component="h2">
        <div>{msg1}°C</div>
        </Typography>
        </Stack>
    </Box>
    </Box>
    </Stack>
        </Container>
        
    )
}