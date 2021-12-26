import React from 'react';
import Container from '@mui/material/Container';
import { Box } from '@mui/system';
import Slider from '@mui/material/Slider';
import { Hook } from "../Hook";
import FormGroup from '@mui/material/FormGroup';
import FormControlLabel from '@mui/material/FormControlLabel';
import Switch from '@mui/material/Switch';
import { Button } from '@mui/material';
var timeout;
export default function Notes() {

  const [name, setName] = Hook("name", "");
  function preventHorizontalKeyboardNavigation(event) {
    if (event.key === 'ArrowLeft' || event.key === 'ArrowRight') {
      event.preventDefault();
    }
  }

  function valuetext(value) {
    return `${value}°`;
  }


  const handleChange = (event, newValue) => {
    console.log(newValue);
  };

  const { value } = 50;

  return (
    <div>
      <Container maxWidth="sm">
      <FormGroup>
      <FormControlLabel control={<Switch onChange={handleChange}/>} label="Label" />
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
        onChangeCommitted	={handleChange}
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
        onChangeCommitted	={handleChange}
      />
      </Box>
      </Container>
      
    </div>
  )
}
