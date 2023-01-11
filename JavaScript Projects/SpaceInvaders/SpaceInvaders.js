var mycanvas = document.getElementById("mycanvas");		// canvas that game takes place in
var ctx = mycanvas.getContext("2d");
var bullets = [];						// array of bullets
var enemies = [];						// array of enemies
var gameover = false;						// bool for gameover state
var gamePaused = false;						// bool for gamepaused state
var die = [];
var restart = document.getElementById("restart");		// restart button from html
var pauseGame = document.getElementById("pause");		// pause button from html
var score = 0;							// score counter
var scoredisplay = document.getElementById("scoredisplay");	// score display
var requestId;
var game;
var spawn = 1000;						// spawn time for enemies
var movementSpeed = 1;						// speed of enemies
var yChange = 0;
var line = {
    xPos: 0,
    yPos: 500,
    height: 20,
    width: 400,
};
var temp = 0;

pauseGame.addEventListener("click", pause);
restart.addEventListener("click", reStart);

/* Function that pauses the game */
function pause() {
    if(gamePaused === true){
        gamePaused = false;
        requestId = window.requestAnimationFrame(gameLoop);
        pauseGame.value = "pause";
    } else {
        gamePaused = true;
        window.cancelAnimationFrame(requestId);
        pauseGame.value = "resume";
    }
}

/* Function that resets game */
function reStart(){
    window.cancelAnimationFrame(requestId);
    score = 0;
    gameover = false;
    enemies = [];
    bullets = [];
    box.xPos = 250;
    box.yPos = 450;
    box.shooting = false;
    box.goLeft = false;
    box.goRight = false;
	temp = 0;
    gameLoop();
}

/* Player object */
var box = {
    xPos: 250,
    yPos: 450,
    height: 20,
    width: 20,
    goLeft: false,
    goRight: false,
    shooting: false,
    die: false,
    canshoot: true,
    
    move: function() {

        if (box.goLeft && box.xPos > 0) {
            box.xPos -= 5;
        }
        if (box.goRight && box.xPos < 480) {
            box.xPos += 5;
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
    this.height = 20;
    this.width = 20;
    this.draw = function() {
        ctx.rect(this.xPos, this.yPos, this.width, this.height);
        ctx.stroke();
    };
    this.moveXaxis = function(yPos) {
        this.xPos -= movementSpeed;
		if (this.xPos <= 0) {
			movementSpeed = -1;
		} else if (this.xPos >= 480) {
			movementSpeed = 1;
		}
        /* if (this.yPos > 500){
            this.toremove = true;
        } */
    };
	this.moveYaxis = function() {
		this.yPos += 10;
	};
}
    
    
// What happens when a key is pressed
document.addEventListener("keydown", function(evt) {
    if (evt.keyCode === 37) {	// K_LEFT
        box.goLeft = true;
    }
    if (evt.keyCode === 39) {	// K_RIGHT
        box.goRight = true;
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
    if (evt.keyCode === 39) {	// K_RIGHT
        box.goRight = false;
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
	
	var x = 20;
	var y = 20;
   
	if (temp == 0) {
		// Spawns enemies
		for (var i = 0; i < 5; i++) {
			for (var k = 0; k < 11; k++) {
				enemies.push(new Enemy(x, y));
				x += 40;
			}
			y += 40;
			x = 20;
		}
		temp = temp + 1;
	}
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

	var p = movementSpeed;
	
    // Draws enemies and handles collision detection with player
    for (var i = 0; i < enemies.length; i++) {
        enemies[i].moveXaxis();
		if (p != movementSpeed) {
			for (var j = 0; j < enemies.length; j++) {
				enemies[j].moveYaxis();
			}
		}
        enemies[i].draw();
        if(isColliding(box, enemies[i])){	// if enemy is colliding with box
            gameover = true;
            alert("Press R to Restart.");
        }
        if(isColliding(line, enemies[i])){	// if enemy is at the end of the line
            gameover = true;
            alert("Press R to Restart.");
        }
		p = movementSpeed;
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