class Road
{
    constructor(src, x ,y , width, height)
    {
        this.img = new Image();
        this.img.src = src;
    }
    show = () => g_ctx.drawImage(this.img, this.x, this.y1, this.width, this.height);
}