import { BrowserRouter as Router, Switch, Route } from "react-router-dom";
import Notes from "./pages/Notes";
import Heating from "./pages/Heating";
import Home from "./pages/Home";
import socketIOClient from "socket.io-client";
import React,{useState,useEffect} from 'react';
import {Modal} from 'react-bootstrap';
import {Button} from 'react-bootstrap';
import Login from "./Login";
import NavbarCollapse from "react-bootstrap/esm/NavbarCollapse";
import { Navbar } from "react-bootstrap";
import { Container } from "react-bootstrap";
import { Nav } from "react-bootstrap";
import { NavDropdown } from "react-bootstrap";
import NavBar from "./pages/NavBar";
const url = "http://127.0.0.1:8090";
const socket = socketIOClient(url);
var timeout;
var msg1=localStorage.getItem('temCurrK');
var msg2=localStorage.getItem('temCurrE');
var msg3=localStorage.getItem('temCurrA');

function App() {

  useEffect(() => {
  socket.on("tempK", (msg1) => {
    console.log(msg1);
    localStorage.setItem('temCurrK', msg1);
  });  
  socket.on("tempE", (msg2) => {
    console.log(msg2);
    localStorage.setItem('temCurrE', msg2);
  }); 
  socket.on("tempA", (msg3) => {
    console.log(msg3);
    localStorage.setItem('temCurrA', msg3);
  }); 
return()=>{
  socket.off("Bye");
}
  
});
  return (
    <Router>
      <Switch>
      <Route exact path="/">
          <Login />
        </Route>
      <Route exact path="/home">
          <Home />
        </Route>
        <Route exact path="/Heating">
          <Heating />
        </Route>
        <Route path="/Solar">
          <Notes />
        </Route>
        
      </Switch>
    </Router>
  );
}

export default App;
