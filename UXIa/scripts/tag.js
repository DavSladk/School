
function showTagVote(e)
{
    let el = e.parentElement.getElementsByTagName('div')[0];
    if (el.className.indexOf("w3-show") == -1) { 
        el.className += " w3-show";
    } else {
        el.className = el.className.replace(" w3-show", "");
    }
}

function hideTagVote(e)
{
    e.className = e.className.replace(" w3-show", "");
}

function onclickUpTag(upVote)
{
    if (upVote.className.indexOf("voted") == -1) { 
        upVote.className = upVote.className
            .replace(" w3-hover-green", " w3-hover-opacity")
            .replace(" w3-white", " w3-green")
            .replace(" w3-text-green", " w3-text-white") + " voted";
    } else {
        upVote.className = upVote.className
            .replace(" voted", "")
            .replace(" w3-hover-opacity", " w3-hover-green")
            .replace(" w3-green", " w3-white")
            .replace(" w3-text-white", " w3-text-green");
    }

    const downVote = upVote.parentElement.getElementsByTagName("button")[1];
    if (downVote.className.indexOf("voted") != -1) { 
        downVote.className = downVote.className
            .replace(" voted", "")
            .replace(" w3-hover-opacity", " w3-hover-red")
            .replace(" w3-red", " w3-white")
            .replace(" w3-text-white", " w3-text-red");
    }
}

function onclickDownTag(downVote)
{
    if (downVote.className.indexOf("voted") == -1) { 
        downVote.className = downVote.className
            .replace(" w3-hover-red", " w3-hover-opacity")
            .replace(" w3-white", " w3-red")
            .replace(" w3-text-red", " w3-text-white") + " voted";
    } else {
        downVote.className = downVote.className
            .replace(" voted", "")
            .replace(" w3-hover-opacity", " w3-hover-red")
            .replace(" w3-red", " w3-white")
            .replace(" w3-text-white", " w3-text-red");
    }

    const upVote = downVote.parentElement.getElementsByTagName("button")[0];
    if (upVote.className.indexOf("voted") != -1) { 
        upVote.className = upVote.className
            .replace(" voted", "")
            .replace(" w3-hover-opacity", " w3-hover-green")
            .replace(" w3-green", " w3-white")
            .replace(" w3-text-white", " w3-text-green");
    }
}

// https://stackoverflow.com/questions/17242144/javascript-convert-hsb-hsv-color-to-rgb-accurately
function HSV2RGB(h, s, v) {
    let r, g, b, i, f, p, q, t;

    i = Math.floor(h * 6);
    f = h * 6 - i;
    p = v * (1 - s);
    q = v * (1 - f * s);
    t = v * (1 - (1 - f) * s);

    switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    return [
        Math.round(r * 255),
        Math.round(g * 255),
        Math.round(b * 255)
    ];
}

function RGB2CSS(rgb) {
    return "rgb(" + rgb.join(", ") + ")";
}

function getTagColor(tag, tags)
{
    const max = tags.reduce((acc, currTag) => currTag.score > acc ? currTag.score : acc, 0);
    const min = tags.reduce((acc, currTag) => currTag.score < acc ? currTag.score : acc, 0);

    const saturation = Math.max(tag.score + tag.myVote, -15) / max;
    const rgb = HSV2RGB(0.460, saturation, 0.59);

    rgb.push(
        tag.score >= 0 ? 1 : 1 / Math.exp(tag.score / min)
    );
    
    console.log(rgb[3]);
    return rgb;
}

function getTagWithVoting(tag, tags)
{
    const rgba = getTagColor(tag, tags);
    const color = RGB2CSS(rgba.slice(0, 3));
    
    const tagEl = document.createElement("div");
    tagEl.className = "w3-dropdown-click w3-white";
    tagEl.style.marginRight = "2px";
    tagEl.style.opacity = rgba[3];

    const tagButton = document.createElement("button");
    tagButton.setAttribute("onclick","showTagVote(this)");
    tagButton.className = "w3-button w3-padding-small w3-tiny w3-round-xxlarge w3-bar-item w3-border";
    tagButton.style.backgroundColor = color;
    tagButton.innerHTML = tag.id;

    const tagDropdown = document.createElement("div");
    tagDropdown.setAttribute("onmouseleave","hideTagVote(this)");
    tagDropdown.className="w3-dropdown-content w3-bar-block dropdown";

    const btnUp = document.createElement("button");
    btnUp.setAttribute("onclick","onclickUpTag(this)");
    btnUp.className = "w3-button w3-hover-green w3-white w3-text-green w3-ripple w3-round-xxlarge w3-border w3-border-green no-padding";
    btnUp.style.width = "100%";
    btnUp.innerHTML = '<i class="fa fa-arrow-up"></i>';

    const middle = document.createElement("div");
    middle.className = "w3-padding-small w3-tiny w3-round-xxlarge w3-bar-item w3-border";
    middle.style.width = "100%";
    middle.innerHTML = tag.id;
    middle.style.backgroundColor = color;

    const btnDown = document.createElement("button");
    btnDown.setAttribute("onclick","onclickDownTag(this)");
    btnDown.className = "w3-button w3-hover-red w3-white w3-text-red w3-ripple w3-round-xxlarge w3-border w3-border-red no-padding";
    btnDown.style.width = "100%";
    btnDown.innerHTML = "<i class='fa fa-arrow-down'></i>";

    tagDropdown.appendChild(btnUp);
    tagDropdown.appendChild(middle);
    tagDropdown.appendChild(btnDown);
    tagEl.appendChild(tagButton);
    tagEl.appendChild(tagDropdown);

    if (tag.myVote == 1) {
        onclickUpTag(btnUp);
    } else if (tag.myVote == -1) {
        onclickDownTag(btnDown);
    }

    return tagEl;
}

function getTag(tag, tags)
{
    const rgba = getTagColor(tag, tags);
    const color = RGB2CSS(rgba.slice(0, 3));

    const tagEl = document.createElement("div");
    tagEl.className = "w3-padding-small w3-tiny w3-round-xxlarge w3-bar-item w3-border";
    tagEl.style.backgroundColor = color;
    tagEl.style.opacity = rgba[3];
    tagEl.style.marginRight = "1px";
    tagEl.innerHTML = tag.id;

    return tagEl;
}
