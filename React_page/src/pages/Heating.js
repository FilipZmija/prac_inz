import React from 'react';
import Typography from '@mui/material/Typography';
import Button from '@mui/material/Button';
import Container from '@mui/material/Container';
import { Stack } from '@mui/material';
import { Divider } from '@mui/material';
import { Box } from '@mui/system';
import { grid } from '@mui/system';
import CircularSlider from '@fseehawer/react-circular-slider';

export default function Create() {
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
          onClick={() => console.log('Heating Kitchen!')}
          variant="contained">
          ON/OFF</Button>
          Manual heating in kitchen
          <CircularSlider
          label="Set temperature in kitchen"
          width={250}
          min={16}
          max={28}
          labelColor="#66c2ff"
          progressColorFrom="#80ccff"
          progressColorTo="#006bb3"
          dataIndex={12}
          data={["16.0" , "16.5" , "17.0" , "17.5" , "18.0", "18.5" , "19.0" , "19.5", "20.0" , "20.5" , "21.0" , "21.5" , "22.0" , "22.5" , "23.0" , "23.5" , "24.0" , "24.5" , "25.0" , "25.5" , "26.0" , "26.5" , "27.0" , "27.5" , "28.0"]}
          knobPosition="bottom"
          appendToValue="°C"
          labelBottom={true}
          onChange={ value => { console.log("K"+value)}}
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
          onClick={() => console.log('Heating Bedroom!')}
          variant="contained">
          ON/OFF</Button>
          Manual heating in bedroom
          <CircularSlider
          label="Set temperature in bedroom"
          width={250}
          min={16}
          max={28}
          labelColor="#66c2ff"
          data={["16.0" , "16.5" , "17.0" , "17.5" , "18.0", "18.5" , "19.0" , "19.5", "20.0" , "20.5" , "21.0" , "21.5" , "22.0" , "22.5" , "23.0" , "23.5" , "24.0" , "24.5" , "25.0" , "25.5" , "26.0" , "26.5" , "27.0" , "27.5" , "28.0"]}
          dataIndex={12}
          knobPosition="bottom"
          appendToValue="°C"
          labelBottom={true}
          onChange={ value => {console.log("A"+value)}}
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
          onClick={() => console.log('Heating Bathroom!')}
          variant="contained">
          ON/OFF</Button>
          Manual heating in bathroom
          <CircularSlider
          label="Set temperature in bathroom"
          width={250}
          min={16}
          max={28}
          dataIndex={12}
          labelColor="#66c2ff"
          data={["16.0" , "16.5" , "17.0" , "17.5" , "18.0", "18.5" , "19.0" , "19.5", "20.0" , "20.5" , "21.0" , "21.5" , "22.0" , "22.5" , "23.0" , "23.5" , "24.0" , "24.5" , "25.0" , "25.5" , "26.0" , "26.5" , "27.0" , "27.5" , "28.0"]}
          knobPosition="bottom"
          appendToValue="°C"
          labelBottom={true}
          onChange={ value => { console.log("E"+value)}}
        />
        </Stack>
      </Box>      
      </Stack>
      </Container>
    
    )
    </div>
  )
}
