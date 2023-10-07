(function() {
    
    function getScript() {
        var textArea = document.getElementById("layScript");
        return textArea.textContent;
    }

    function parsePoint(pt) {
        var sp = pt.split('/');
        return {
            x: parseInt(sp[0]),
            y: parseInt(sp[1])
        };
    }

    function parseLine(l) {
        var toks = l.split(',');
        var o = {};
        toks.forEach(function(t) {
            var kv = t.split('=');
            if (kv.length > 1)
            {
                var k = kv[0];
                var v = kv[1];

                var p = k.match(/P(\d+)/);
                if (p) {
                    if (!o.p)
                        o.p = [];
                    o.p[parseInt(p[1])] = parsePoint(v);
                } else if (k === 'POS') {
                    o.pos = parsePoint(v);
                } else if (k === 'CENTER') {
                    o.center = parsePoint(v);
                } else
                    o[k.toLowerCase()] = v;
            }
            else
                o[kv[0].toLowerCase()] = true;
        });
        return o;
    }

    function parseScript(s) {
        var lines = s.split(';');
        lines = lines.map(function(l) {return l.trim();});
        lines = lines.filter(function(l) {return l.length > 0;});
        return lines.map(parseLine);
    }

    function getLayerColor(l) {
        return ["","blue","red","green","yellow","","","white"][l];
    }

    function renderSmdPad(o) {
        var transform = '';
        transform += 'translate(' + o.pos.x + ',' + o.pos.y + ') ';

        if (o.rotation !== undefined) {
            transform += 'rotate(' + o.rotation / 100 + ')';
        }

        var svg = '';
        svg += '<g transform="' + transform + '" fill="' + getLayerColor(o.layer) + '">';
        svg += '<rect x="0" y="0" width="' + o.size_x + '" height="' + o.size_y + '"/>';
        svg += '</g>';
        return svg;
    }

    function renderZone(o) {
        var svg = '';
        svg += '<polygon points="';
        svg += o.p.reduce(function (s, p) {return s+=p.x+','+p.y + ' ';}, "");
        svg += '" stroke="' + getLayerColor(o.layer) + '" fill="' + getLayerColor(o.layer) + '" stroke-width=' + o.width + ' />';
        return svg;
    }

    function renderTrack(o) {
        var svg = '';
        svg += '<polyline points="';
        svg += o.p.reduce(function (s, p) {return s+=p.x+','+p.y + ' ';}, "");
        svg += '" fill="none" stroke="' + getLayerColor(o.layer) + '" stroke-width=' + o.width + ' />';
        return svg;
    }

    function renderPad(o) {
        var cx = o.pos.x;
        var cy = o.pos.y;
        var r = o.size/2;
        return '<circle cx="' + cx + '" cy="' + cy + '" r="' + r + '" fill="' + getLayerColor(o.layer) + '" />';
    }

    function renderPadDrill(o) {
        var cx = o.pos.x;
        var cy = o.pos.y;
        var r = o.drill/2;
        return '<circle cx="' + cx + '" cy="' + cy + '" r="' + r + '" fill="black" />';
    }

    function renderCircle(o) {
        var cx = o.center.x;
        var cy = o.center.y;
        var r = o.radius;
        return '<circle cx="' + cx + '" cy="' + cy + '" r="' + r + '" stroke="' + getLayerColor(o.layer) + '" stroke-width="' + o.width + '" fill="none" />';
    }

    function createSvg(parsed) {
        var svg = '<svg  viewBox="0 0 2000000 2000000" style="background-color:black" xmlns="http://www.w3.org/2000/svg">';
        svg += '<style>text { font-family: sans-serif; }</style>';
        var layers = ["","","","","","","",""];
        var drill = '';
        for (var i = 0; i < parsed.length; i++) {
            var obj = parsed[i];
            if (obj.pad) {
                layers[obj.layer] += renderPad(obj);
                drill += renderPadDrill(obj);
            } else if (obj.smdpad) {
                layers[obj.layer] += renderSmdPad(obj);
            } else if (obj.track) {
                layers[obj.layer] += renderTrack(obj);
            } else if (obj.zone) {
                layers[obj.layer] += renderZone(obj);
            } else if (obj.circle) {
                layers[obj.layer] += renderCircle(obj);
            } else if (obj.text) {
                var text = obj.text.substr(1, obj.text.length - 2);
                var rot = obj.rotation !== undefined ? obj.rotation : 0;
                layers[obj.layer] += '<text x="' + obj.pos.x + '" y="' + obj.pos.y + '" style="font-size: ' + obj.height*10 + '; fill: ' + getLayerColor(obj.layer) + '" transform="rotate('  + rot + ', ' + obj.pos.x + ', ' + obj.pos.y + ')" >' + text + '</text>';
            } else {
                console.log('unk', obj);
            }
        }
        for (var i = layers.length-1; i >= 0; i--) {
            svg += '<!-- layer ' + i + ' -->';
            svg+=layers[i];
        }
        svg += '<!-- drill -->';
        svg += drill;
        svg+='</svg>';
        return svg;
    }

    function update() {
        var parsed = parseScript(getScript());
        document.getElementById('parsed').innerText = JSON.stringify(parsed, null, 2);
        document.getElementById('viewer').innerHTML = createSvg(parsed);
    }

    window.onload = function() {
        update();
        var textArea = document.getElementById("layScript");
        textArea.oninput = function() {
            update();
        }
    }
})();