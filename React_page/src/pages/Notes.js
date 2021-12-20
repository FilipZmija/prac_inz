import React from 'react';
import Typography from '@mui/material/Typography';
import Button from '@mui/material/Button';
import Container from '@mui/material/Container';
import { Stack } from '@mui/material';
import { Divider } from '@mui/material';
import { Box } from '@mui/system';
import { grid } from '@mui/system';
import CircularSlider from '@fseehawer/react-circular-slider';
import Slider from '@mui/material/Slider';

export default function Notes() {

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

  return (
    <div>
      <Container maxWidth="sm">
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
        defaultValue={30}
        getAriaValueText={valuetext}
        valueLabelDisplay="auto"
        step={1}
        min={30}
        max={150}
        onChangeCommitted	={handleChange}
      />
      </Box>
      </Container>
      
    </div>
  )
}
