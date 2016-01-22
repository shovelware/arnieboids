//This is the page that doxygen will display as the main page in the generated html.
/*! \mainpage ArnieBoids - Main Page
*
* \section tldr_sec tl;dr
* It's a top-down space shooter with a focus on artificial intelligence.
*
* \section start_sec Where do I start?
* Take a look at the [Class Hierarchy](inherits.html). That should give you an idea of the project structure.
*
* \section brief_sec Brief
* This section is the brief we received for the project. Word for word.
*
* \subsection controls_subsec Controls
* - The player’s ship is controlled using the left, right and up arrows. 
* - Left and right arrows cause the ship to turn anticlockwise and clockwise respectively. 
* - The up arrow applies thrust to the ship in the direction it is pointing. 
* - The space bar fires a bullet.
* 
* \subsection factories_subsec Factories
* 
* - These produce predators while actively trying to evade the player. 
* - They can survive four hits from the player. 
* - When the player’s ship is within range they will take evasive behaviour, otherwise they wander at random. 
* - If factories meet they will flock. 
* - A factory will fire interceptor guided missiles at the player once the player is within range. 
* - Interceptor missiles have the same maximum speed as the player and will explode if they do not reach the player within 10 seconds.
* - A factory can only have five interceptor missiles “live” at a time.
*
* \subsection swarmboids_subsec Swarm Boids
* - These swarm in groups and tend towards the players ship
* - Once they are within range of the ship they then plot an intercept course for the ship.
*
* \subsection predator_subsec Predator Ships
* - These seek out the player and intelligently engage them.
* - They have the same firepower as the player ship.
* - Predators move in flocks and can also collect “power-ups”!
*
* \subsection obstacles_subsec Obstacles
* - Exactly what it says on the tin
*
* \subsection powerups_subsec Power-Ups
* - To be decided by individual game designers.
*
* \subsection other_subsec Other Requirements
* - Friction operates in the game and any moving object will slow down over time if thrust is not applied to it.
* - All entities try to avoid obstacles.
* - Collision with an obstacle results in death.
* - The screen should scroll so that the player’s ship is always in the centre of the screen.
* - The game is wrap around with the total area of play 9 times that of a single screen.
* - Radar will be provided for the player.
*/