window.onload = function() {
    var img = document.getElementById('moving-image');

    // 初始随机位置
    moveImage();

    img.addEventListener('mouseover', moveImage);

    function moveImage() {
        var maxX = window.innerWidth - img.clientWidth;
        var maxY = window.innerHeight - img.clientHeight;

        var randX = Math.random() * maxX;
        var randY = Math.random() * maxY;

        img.style.left = randX + 'px';
        img.style.top = randY + 'px';
    }
};