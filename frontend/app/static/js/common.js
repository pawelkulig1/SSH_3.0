function send_request(command, value) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
        }
    };
    xhttp.open("GET", "api?" + command + "=" + value, true);
    xhttp.send();
}

