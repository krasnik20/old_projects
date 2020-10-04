class Car
{
    constructor(src, width, height, forwardSpeed, backwardSpeed, turnSpeed)
    {
        this.img = new Image();
        this.img.src = src;
        this.height = height;
        this.width = width;
        this.x = g_canvas.width / 2 - width / 2;
        this.y = g_canvas.height / 2 - width / 2;
        this.forwardSpeed = forwardSpeed;
        this.backwardSpeed = backwardSpeed;
        this.turnSpeed = turnSpeed;
    }
    move()
    {
        if (g_direction == LEFT) this.x -= this.turnSpeed; 
        if (g_direction == RIGHT) this.x += this.turnSpeed;
        if (g_direction == UP) this.y -= this.forwardSpeed;
        if (g_direction == DOWN) this.y += this.backwardSpeed;
        if (this.y > g_canvas.height - 20 - this.height) this.y -= this.backwardSpeed; 
        if (this.y < 50) this.y += this.forwardSpeed; 
        if (this.x < 10) this.x += this.turnSpeed; 
        if (this.x > g_canvas.width - 10 - this.width) this.x -= this.turnSpeed;
        g_ctx.drawImage(this.img, this.x, this.y, this.width, this.height);
    }
    collision(enemy)
    {
        if ((this.x <= enemy.cornerX && this.x >= enemy.x) && ((this.cornerY >= enemy.y && this.cornerY <= enemy.cornerY) || (this.y >= enemy.y && this.y <= enemy.cornerY))) return true;
        if ((this.cornerX <= enemy.cornerX && this.cornerX >= enemy.x) && ((this.cornerY >= enemy.y && this.cornerY <= enemy.cornerY) || (this.y >= enemy.y && this.y <= enemy.cornerY))) return true;
        return false;
    }
}