import React, { Component } from "react";
import "./loggin.css";
import { Container } from "@mui/material";
import Home from "./pages/Home";
import { Redirect } from "react-router-dom";
import { useEffect } from 'react';
import { Navbar } from "react-bootstrap";

var logged=localStorage.getItem('logged');

const handleRedirect = () => {
    return 
  };
class Login extends Component {
    
    
  handleSubmit = (e) => {
    e.preventDefault();
    console.log(e.target.email.value);
    
    if (!e.target.email.value) {
      alert("Email is required");
    } else if (!e.target.email.value) {
      alert("Valid email is required");
    } else if (!e.target.password.value) {
      alert("Password is required");
    } else if (
      e.target.email.value === "me@home.com" &&
      e.target.password.value === "12345"
    ) {
      alert("Successfully logged in");
      logged=1;
      localStorage.setItem("logged", logged);
      window.location.href = "http://localhost:3000/Home";
      e.target.email.value = "";
      e.target.password.value = "";
      

    } else {
      alert("Wrong email or password combination");
    }
  };

  
    
    render() {
    return (
        
      <div className="App">
          <Container>
        <form className="form" onSubmit={this.handleSubmit}>
          <div className="input-group">
            <label htmlFor="email">Login</label>
            <input type="email" name="email" placeholder="student@agh.edu.pl" />
          </div>
          <div className="input-group">
            <label htmlFor="password">Hasło</label>
            <input type="password" name="password" />
          </div>
          <button className="primary" >Login</button>
        </form>
        </Container>
      </div>
      
    );
  }
}

export default Login;
