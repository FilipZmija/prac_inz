import * as React from 'react';
import Box from '@mui/material/Box';
import Button from '@mui/material/Button';
import Dialog from '@mui/material/Dialog';
import DialogActions from '@mui/material/DialogActions';
import DialogContent from '@mui/material/DialogContent';
import DialogContentText from '@mui/material/DialogContentText';
import DialogTitle from '@mui/material/DialogTitle';
import CircularSlider from '@fseehawer/react-circular-slider';
import { Stack } from '@mui/material';
import { Container } from '@mui/material';
import socketIOClient from "socket.io-client";
const url = "http://127.0.0.1:8090";
const socket = socketIOClient(url);

export default function MaxWidthDialog() {
  const [open, setOpen] = React.useState(false);
  const [fullWidth, setFullWidth] = React.useState(true);
  const [maxWidth, setMaxWidth] = React.useState('sm');
  var temA=localStorage.getItem('temA');
  var timeout1;
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
  

  const handleClickOpen = () => {
    setOpen(true);
  };

  const handleClose = () => {
    setOpen(false);
  };

  const handleMaxWidthChange = (event) => {
    setMaxWidth(
      // @ts-expect-error autofill of arbitrary value is not handled.
      event.target.value,
    );
  };

  const handleFullWidthChange = (event) => {
    setFullWidth(event.target.checked);
  };

  return (
    <React.Fragment>
      <Container>
      <Button variant="text" color="error" size="large" fullWidth={true} 
      sx={{
        fontSize:'30px'
      }}
       onClick={handleClickOpen}>
        Bathroom
      </Button>
      </Container>
      
      <Dialog
        open={open}
        onClose={handleClose}
      >
        <DialogTitle>Temperature setup</DialogTitle>
        <DialogContent>
        <Stack
          direction="column"
          justifyContent="center"
          alignItems="center"
          spacing={3}
        >
          <DialogContentText>
            You can change temperature in bathroom here
          </DialogContentText>
          <CircularSlider
          label=" "
          width={150}
          min={16}
          max={28}
          labelColor="#66c2ff"
          progressColorFrom="#80ccff"
          progressColorTo="#006bb3"
          dataIndex={temA}
          valueFontSize="2rem"
          data={["16.0" , "16.5" , "17.0" , "17.5" , "18.0", "18.5" , "19.0" , "19.5", "20.0" , "20.5" , "21.0" , "21.5" , "22.0" , "22.5" , "23.0" , "23.5" , "24.0" , "24.5" , "25.0" , "25.5" , "26.0" , "26.5" , "27.0" , "27.5" , "28.0"]}
          knobPosition="bottom"
          appendToValue="°C"
          labelBottom={true}
          onChange={handleChangeA}
        />
        </Stack>
        </DialogContent>
        <DialogActions>
          <Button onClick={handleClose}>Close</Button>
        </DialogActions>
      </Dialog>
    </React.Fragment>
  );
}