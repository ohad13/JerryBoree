const Navbar = () => {
    return (
      <nav className="navbar">
        <a href='/'><h1>React Blog</h1></a>
        <div className="links">
          <a href="/">Home</a>
          <a href="/create" style={{ 
            color: 'white', 
            backgroundColor: '#f1356d',
            borderRadius: '8px' 
          }}>New Blog</a>
          <a href='/help'>Help</a>
        </div>
      </nav>
    );
  }
   
  export default Navbar;