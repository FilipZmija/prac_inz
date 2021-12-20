import { BrowserRouter as Router, Switch, Route } from 'react-router-dom'
import Notes from './pages/Notes'
import Heating from './pages/Heating'

function App() {
  return (
    <Router>
      <Switch>
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
