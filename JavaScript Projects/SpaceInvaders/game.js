var mycanvas = document.getElementById("mycanvas");		// canvas that game takes place in
var ctx = mycanvas.getContext("2d");
var bullets = [];						// array of bullets
var enemies = [];						// array of enemies
var gameover = false;						// bool for gameover state
var gamePaused = false;						// bool for gamepaused state
var die = [];
var restart = document.getElementById("restart");		// restart button from html
var pauseGame = document.getElementById("pause");		// pause button from html
var dropdown = document.getElementById("dropdown");		// dropdown menu of difficulties
var change = document.getElementById("change");			// change button from html
var score = 0;							// score counter
var scoredisplay = document.getElementById("scoredisplay");	// score display
var requestId;
var game;
var spawn = 1000;						// spawn time for enemies
var movementSpeed = 1;						// speed of enemies
// var wave1 = setInterval(enemySpawn, spawn);			// wave intervals
var line = {
    xPos: 0,
    yPos: 500,
    height: 20,
    width: 400,
};

change.addEventListener("click", assignDifficulty);
pauseGame.addEventListener("click", pause);
restart.addEventListener("click", reStart);

/* Function that pauses the game */
function pause() {
    if(gamePaused === true){
        gamePaused = false;
        // wave1 = setInterval(enemySpawn, spawn);
        requestId = window.requestAnimationFrame(gameLoop);
        pauseGame.value = "pause";
    } else {
        gamePaused = true;
        // clearInterval(wave1);
        window.cancelAnimationFrame(requestId);
        pauseGame.value = "resume";
    }
}

/* Function that resets game */
function reStart(){
    // clearInterval(wave1);
    window.cancelAnimationFrame(requestId);
    score = 0;
    gameover = false;
    enemies = [];
    bullets = [];
    box.xPos = 200;
    box.yPos = 400;
    box.shooting = false;
    box.goLeft = false;
    box.goRight = false;
    // wave1 = setInterval(enemySpawn, spawn);
    gameLoop();
}

/* Function that changes difficulty.
/* Change in difficulty results in change in spawn time 
/* and movement speed for enemies */
function assignDifficulty(){
    if (dropdown.value == "SuperEasy") {
		spawn = 1000;
		movementSpeed = 1;
    }
    if (dropdown.value == "Easy") {
		spawn = 100;
		movementSpeed = 1;
    }
    if (dropdown.value == "Medium") {
		spawn = 1;
		movementSpeed = 1;
    }
    if (dropdown.value == "Hard") {
		spawn = 1;
		movementSpeed = 10;
    }
    if (dropdown.value == "Impossible") {
		spawn = 1;
		movementSpeed = 100;
    }
    reStart();
}

/* Player object */
var box = {
    xPos: 200,
    yPos: 400,
    height: 20,
    width: 20,
    goLeft: false,
    goRight: false,
    goUp: false,
    goDown: false,
    shooting: false,
    die: false,
    canshoot: true,
    
    move: function() {

        if (box.goLeft && box.xPos > 0) {
            box.xPos -= 5;
        }
        if (box.goRight && box.xPos < 380) {
            box.xPos += 5;
        }
        if (box.goUp && box.yPos > 0) {
            box.yPos -= 5;
        }
        if (box.goDown && box.yPos < 480) {
            box.yPos += 5;
        }

    },
    shoot: function() {
        if (box.shooting && box.canshoot) {
            bullets.push(new Bullet(box.xPos, box.yPos));
            setTimeout(function(){
                box.canshoot = true;
            }, 300);
            box.canshoot = false;
        }
    },
    draw: function() {
        ctx.rect(box.xPos, box.yPos, this.width, this.height);
        ctx.stroke();
    }
};

// Bullet Constructor
function Bullet(xPos, yPos) {
    this.xPos = xPos + 9;
    this.yPos = yPos;
    this.height = 10;
    this.width = 2;
    this.draw = function() {
        ctx.rect(this.xPos, this.yPos, this.width, this.height);
        ctx.stroke();
    };
    this.move = function() {
        this.yPos -= 10;
        if(this.yPos < -5){
            this.toremove = true;
        }
        if(this.yPos < 0){
            return false;
        } else {
            return true;
        }
    };
    this.toremove = false;
}

// Enemy Constructor
function Enemy(xPos, yPos) {
    this.xPos = xPos;
    this.yPos = yPos;
    this.height = 30;
    this.width = 30;
    this.draw = function() {
        ctx.rect(this.xPos, this.yPos, this.width, this.height);
        ctx.stroke();
    };
    this.move = function() {
        this.xPos -= 0;
        this.yPos += movementSpeed;
        if(this.yPos > 500){
            this.toremove = true;
        }
    };
}

// Spawns enemies
/* function enemySpawn() {
    var tempRand = Math.random() * mycanvas.width;
    if (tempRand > 380) {
		tempRand = tempRand - 20;
    }
    enemies.push(new Enemy(tempRand - 5, 0));
} */
    
    
// What happens when a key is pressed
document.addEventListener("keydown", function(evt) {
    if (evt.keyCode === 37) {	// K_LEFT
        box.goLeft = true;
    }
    if (evt.keyCode === 38) {	// K_UP
        box.goUp = true;
    }
    if (evt.keyCode === 39) {	// K_RIGHT
        box.goRight = true;
    }
    if (evt.keyCode === 40) {	// K_DOWN
        box.goDown = true;
    }
    if (evt.keyCode === 32) {	// K_SPACE
        box.shooting = true;
    }
    if (evt.keyCode === 80){	// P (PAUSE)
		pause();
    }
    if (evt.keyCode === 82) {	// R (RESTART)
        reStart();
    }
});

// What happens when a key is released
document.addEventListener("keyup", function(evt) {
    if (evt.keyCode === 37) {	// K_LEFT
        box.goLeft = false;
    }
    if (evt.keyCode === 38) {	// K_UP
        box.goUp = false;
    }
    if (evt.keyCode === 39) {	// K_RIGHT
        box.goRight = false;
    }
    if (evt.keyCode === 40) {	// K_DOWN
        box.goDown = false;
    }
    if (evt.keyCode === 32) {	// K_SPACE
        box.shooting = false;
    }
    
});

// main game loop
function gameLoop() {
    ctx.beginPath();
    ctx.clearRect(0, 0, mycanvas.width, mycanvas.height);
    box.move();
    box.draw();
    box.shoot();
   
    // Draws bullets and handles collision detection with enemy
    for (var j = 0; j < bullets.length; j++) {
        bullets[j].move();
        bullets[j].draw();
        for(var k = 0; k < enemies.length; k++){
            if(isColliding(bullets[j], enemies[k])){
                enemies[k].toremove = true;
                bullets[j].toremove = true;
                score = score + 100;
            } 
        }   
    }

    // Draws enemies and handles collision detection with player
    for (var i = 0; i < enemies.length; i++) {
        enemies[i].move();
        enemies[i].draw();
        if(isColliding(box, enemies[i])){	// if enemy is colliding with box
            gameover = true;
            alert("Press R to Restart.");
        }
        if(isColliding(line, enemies[i])){	// if enemy is at the end of the line
            gameover = true;
            alert("Press R to Restart.");
        }
    }
    
    garbagecollector();
    scoredisplay.innerHTML = "score: " + score;
    if (gameover === false) {
    	requestId = window.requestAnimationFrame(gameLoop);
    } else {
        window.cancelAnimationFrame(requestId);
        gameover;
    }
}

// Removes garbage bullets and enemies
function garbagecollector(){
    for (var i = 0; i < bullets.length; i++) {
        if(bullets[i].toremove === true){
            bullets.splice(i, 1);
        }
    }
    for (var i = 0; i < enemies.length; i++){
        if(enemies[i].toremove === true){
            enemies.splice(i, 1);
        }
    }
}

// Collision detection
function isColliding(thing1, thing2){
    console.log(thing1, thing2);
    var isLeft = thing2.xPos + thing2.width < thing1.xPos;
    var isRight = thing2.xPos > thing1.xPos + thing1.width;
    var isBelow = thing2.yPos + thing2.height < thing1.yPos;
    var isAbove = thing2.yPos > thing1.yPos + thing1.height;
    return !(isRight||isLeft||isAbove||isBelow);
}

gameLoop();