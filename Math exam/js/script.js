var g_canvas = document.getElementById("example");
g_canvas.width = window.innerWidth;
g_canvas.height = window.innerHeight; 
var g_ctx = g_canvas.getContext("2d");
var g_counter = 
{
    count: 2,
    width: g_canvas.width / 6,
    draw: function()
    {
        g_ctx.font = "Bold " + g_canvas.width / 50 + "pt Arial";
        g_ctx.fillStyle = "#111111";
        g_ctx.fillRect(g_canvas.width / 2 - this.width/2 , 0, this.width, 50);
        g_ctx.fillStyle = "#ffffff";
        g_ctx.fillText('Score:' + this.count, g_canvas.width / 2 - this.width / 2 + 5, 40);
    }
}
var currentCar;
var LEFT = 65;
var RIGHT = 68;
var UP = 87;
var DOWN = 83;
var ANOTHERLEFT = 37;
var ANOTHERRIGHT = 39;
var ANOTHERUP = 38;
var ANOTHERDOWN = 40;
var STAND = 0;
var FPS = 60;
var g_direction = STAND;
var g_intervalId;

var g_pauseButton = 
{
    x: g_canvas.width - 50,
    y: 0,
    width: 50,
    height: 50,
    img: new Image(),
    imgContinue: new Image(),
    imgStop: new Image(),
    sx: 0,
    draw: function() { g_ctx.drawImage(this.img, this.x, this.y, this.width, this.height) },
    pause: function()
    {
        document.body.onclick = function(event) {
            if (event.clientX > g_canvas.width - 60 && event.clientY < 60) startGame();
        };
        this.img = this.imgContinue;
        this.draw();
        clearInterval(g_intervalId);
    },
    init: function() 
    {   
        this.imgStop.src = "images/pause.png";
        this.imgContinue.src = "images/continue.png";
        this.img = this.imgStop;
        document.body.onclick = function (event) {
            if (g_pauseButton.isTouched(event.clientX, event.clientY))
                g_pauseButton.pause();
        }
    },
    isTouched: function(x, y) { return x > this.x && x < this.x + this.width && y > this.y && y < this.y + this.height  ? true : false }
}
var enemies = [];
var helper;
var road = 
{
    img: new Image (),
    show: function() 
    { 
        g_ctx.drawImage(this.img, 0, 0, g_canvas.width, g_canvas.height); 
    }
}
road.img.src = "images/road.png";
gameInit();

