
class TableRow {
    constructor(id, element) {
        this.m_clicked = false;
        this.m_id = id;
        this.m_element = element
        TableRow.tableArr.push(this);
    }

    toggle() {
        if (this.m_clicked == true) {
            this.m_element.style.backgroundColor = "#fff";
        }
        else {
            this.m_element.style.backgroundColor = "#007bff26";
        }
        this.m_clicked = !this.m_clicked;

    }
};
TableRow.tableArr = [];
function row_click(id) {
    console.log(id);

    temp = TableRow.tableArr.filter(value => value.m_id == id);
    if (temp.length > 0) {
        temp[0].toggle();
    }
    else {
        temp = new TableRow(id, document.getElementById("key_nr_" + id));
        temp.toggle();
    }
    selected = TableRow.tableArr.filter(value => value.m_clicked == true);
    if (selected.length > 0) {
        document.getElementById("selected_items_indicator").innerText = selected.length + " Selected";
        document.getElementById("delete_button").disabled = false;
        document.getElementById("prolong_button").disabled = false;
        document.getElementById("renewal_button").disabled = false;
        document.getElementById("disable_button").disabled = false;
        document.getElementById("add_button").disabled = true;
    }
    else {
        document.getElementById("selected_items_indicator").innerText = "0 Selected";
        document.getElementById("delete_button").disabled = true;
        document.getElementById("prolong_button").disabled = true;
        document.getElementById("renewal_button").disabled = true;
        document.getElementById("disable_button").disabled = true;
        document.getElementById("add_button").disabled = false;
    }
}

function action(action_id) {
    selected = TableRow.tableArr.filter(value => value.m_clicked == true);
    command = ""
    switch (action_id) {
        case 0:
            command = "delete"; break
        case 1:
            command = "force_renewal"; break
        case 2:
            command = "disable"; break
        case 3:
            command = "prolong"; break
    }

    for (i = 0; i < selected.length; i++) {
        send_request(command, selected[i].m_id)
    }
}

function send_request(command, value) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
        }
    };
    xhttp.open("GET", "api?" + command + "=" + value, true);
    xhttp.send();
}

function add_button_clicked() {
    document.getElementById("adding_new_key").style.position = "relative";
    document.getElementById("adding_new_key").style.visibility = "visible";
}

function close_key() {
    document.getElementById("adding_new_key").style.position = "absolute";
    document.getElementById("adding_new_key").style.visibility = "hidden";
}

function add_key(key) {
    pub_key = document.getElementById("new_public_key").value;
    console.log(pub_key)
    send_request("add_key", pub_key);
}        