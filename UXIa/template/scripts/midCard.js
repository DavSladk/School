function showTagVote(e)
{
    let el = e.parentElement.getElementsByTagName('div')[0];
    console.log("showVote");
    if (el.className.indexOf("w3-show") == -1) { 
        el.className += " w3-show";
      } else {
        el.className = el.className.replace(" w3-show", "");
      }
}

function hideTagVote(e)
{
    let el = e;
    console.log("hideVote");
    if (el.className.indexOf("w3-show") == -1) { 
        el.className += " w3-show";
      } else {
        el.className = el.className.replace(" w3-show", "");
      }
}

function onclickUpTag(el)
{
    if (el.className.indexOf("w3-hover-white") == -1) { 
        el.className = el.className.replace(" w3-hover-green", " w3-hover-white");
        el.className = el.className.replace(" w3-green", "");
        el.className += " w3-text-green";
    } else {
        el.className = el.className.replace(" w3-hover-white", " w3-hover-green");
        el.className = el.className.replace(" w3-text-green", "");
        el.className += " w3-green";
    }

    let red=el.parentElement.getElementsByTagName("button")[2];
    if (red.className.indexOf("w3-hover-white") == -1) { 
        red.className = red.className.replace(" w3-hover-red", " w3-hover-white");
        red.className = red.className.replace(" w3-red", "");
        red.className += " w3-text-red";
    }
}

function onclickDownTag(el)
{
    if (el.className.indexOf("w3-hover-white") == -1) { 
        el.className = el.className.replace(" w3-hover-red", " w3-hover-white");
        el.className = el.className.replace(" w3-red", "");
        el.className += " w3-text-red";
    } else {
        el.className = el.className.replace(" w3-hover-white", " w3-hover-red");
        el.className = el.className.replace(" w3-text-red", "");
        el.className += " w3-red";
    }

    let green=el.parentElement.getElementsByTagName("button")[0];

    if (green.className.indexOf("w3-hover-white") == -1) { 
        green.className = green.className.replace(" w3-hover-green", " w3-hover-white");
        green.className = green.className.replace(" w3-green", "");
        green.className += " w3-text-green";
    }
}

function onclickUp(el)
{
    if (el.className.indexOf("w3-text-green") == -1) {
        el.className = el.className.replace(" w3-text-white", " w3-text-green");
        el.className = el.className.replace(" w3-green", "");
        let at = Number(el.getAttribute("votes"))-1;
        el.setAttribute("votes", at);
        el.innerHTML = '<i class="fa fa-arrow-up"></i> ' + at;
    } else {
        el.className = el.className.replace(" w3-text-green", " w3-text-white");
        el.className += " w3-green";
        let at = Number(el.getAttribute("votes"))+1;
        el.setAttribute("votes", at);
        el.innerHTML = '<i class="fa fa-arrow-up"></i> ' + at;
    }

    let red = el.parentElement.getElementsByTagName("button")[1];
    if (red.className.indexOf("w3-text-red") == -1) {
        red.className = red.className.replace(" w3-text-white", " w3-text-red");
        red.className = red.className.replace(" w3-red", "");
        let at = Number(red.getAttribute("votes"))-1;
        red.setAttribute("votes", at);
        red.innerHTML = '<i class="fa fa-arrow-down"></i> ' + at;
    }
}

function onclickDown(el)
{
    if (el.className.indexOf("w3-text-red") == -1) {
        el.className = el.className.replace(" w3-text-white", " w3-text-red");
        el.className = el.className.replace(" w3-red", "");
        let at = Number(el.getAttribute("votes"))-1;
        el.setAttribute("votes", at);
        el.innerHTML = '<i class="fa fa-arrow-down"></i> ' + at;
    } else {
        el.className = el.className.replace(" w3-text-red", " w3-text-white");
        el.className += " w3-red";
        let at = Number(el.getAttribute("votes"))+1;
        el.setAttribute("votes", at);
        el.innerHTML = '<i class="fa fa-arrow-down"></i> ' + at;
    }

    let green = el.parentElement.getElementsByTagName("button")[0];
    if (green.className.indexOf("w3-text-green") == -1) {
        green.className = green.className.replace(" w3-text-white", " w3-text-green");
        green.className = green.className.replace(" w3-green", "");
        let at = Number(green.getAttribute("votes"))-1;
        green.setAttribute("votes", at);
        green.innerHTML = '<i class="fa fa-arrow-up"></i> ' + at;
    }
}

function onclickOpen(el)
{
    // TO DO
}

function onclickPin(el)
{
    if (el.className.indexOf("w3-grey") == -1) {
        el.className += " w3-grey";
        el.parentElement.parentElement.setAttribute("pinned","");
    } else {
        el.className = el.className.replace(" w3-grey", "");
        el.parentElement.parentElement.removeAttribute("pinned");
    }
}
