function my_function(message) {
    console.log("from Utilities.js: " + message);
}

function initializeInactivityTimer(dotnetHelper) {
    var timer;
    document.onmousemove = resetTimer;
    document.onkeypress = resetTimer;

    function resetTimer() {
        clearTimeout(timer);
        timer = setTimeout(logout, 3000)
    }

    function logout() {
        dotnetHelper.invokeMethodAsync("Logout")
    }

}