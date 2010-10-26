var colors = ["#cf4b4b",
              "#bbbbbb",
              "#4da74d",
              "#6f0a93",
			  "#EF8B8B",
			  "#027364",
			  "#000000",
			  "#6f0a93",
              "#3e99d7"];

var fillColors = ["#ffffff"];


function drawGraph(graph) {
	$('#graph-container').append($('<span></span>').html(graph.name).css({marginLeft: "70px"}));
	var sel = $('<div></div>');
	$(sel).css({width: "800px", height: "500px" });

	var table = $('<table></table>');	
	var row = $('<tr></tr>');	
	var graphCell = $('<td></td>');	
	var sourceCell = $('<td></td>');
	$(table).append(row);
	$(row).append(graphCell);
	$(row).append(sourceCell);

	var sourceText = $('<textarea></textarea>').attr(
		{spellcheck: false}).css({width: "500px", 
								  height: "400px", 
								  marginLeft: "20px",
								  color: "#777777"
								 }).html(graph.source);

	sourceCell.append(sourceText);
	graphCell.append(sel);

	$('#graph-container').append(table);
	$('#graph-container').append($('<br/>'));
	$('#graph-container').append($('<br/>'));
	$('#graph-container').append($('<br/>'));

	var dsets = graph["dataSet"];
    var sets = [];
    for (var i = 0; i < dsets.length; i++) {
        var dataRadius = 2.0;
		var isTracing = dsets[i].name.indexOf("trace_") == 0;
        sets[i] = {
            lines: { show: true },
            points: { show: true,
                      fillColor: isTracing ? "#ff0000" : "#ffffff"
                    },
            borderWidth: 1.5,
            borderColor: "#BEBEBE",
            markingsLineWidth: 0.75,
            hoverable: true,
            clickable: true,
            label: dsets[i].name,
            data: dsets[i].data,
            color: colors[i],
            shadowSize: 0
        };
    }
    var options = { 
		yaxis: {
            min: 0,
            tickFormatter:
            function (v, axis) { return v + "ms"; }
        },
        xaxis: {
            tickFormatter:
            function (v, axis) {
                v = Math.round(v);
                if (!(v in graph.runTimes))
                    return '';
                return graph.runTimes[v].substr(
					0, 
					graph.runTimes[v].indexOf(','));
            }
        },
        legend:
        {
            show: true
        },
        grid:
        {
            hoverable: true,
            clickable: true
        }
    };


    function showToolTip(x, y, contents) {
        var tipWidth = 165;
        var tipHeight = 75;
        var xOffset = 5;
        var yOffset = 5;
        var ie = document.all && !window.opera;
        var iebody = (document.compatMode == "CSS1Compat")
            ? document.documentElement
            : document.body;
        var scrollLeft = ie ? iebody.scrollLeft : window.pageXOffset;
        var scrollTop = ie ? iebody.scrollTop : window.pageYOffset;
        var docWidth = ie ? iebody.clientWidth - 15 : window.innerWidth - 15;
        var docHeight = ie ? iebody.clientHeight - 15 : window.innerHeight - 18;
        y = (y + tipHeight - scrollTop > docHeight)
            ? y - tipHeight - 5 - (yOffset * 2)
            : y; // account for bottom edge
        
        // account for right edge
        if (x + tipWidth - scrollLeft > docWidth) {
            $('<div id="tooltip">' + contents + '<\/div>').css( 
				{  top: y + yOffset,
				   right: docWidth - x + xOffset
				}).appendTo("body").fadeIn(200);
        } else {
            $('<div id="tooltip">' + contents + '<\/div>').css( 
				{
					top: y + yOffset,
					left: x + xOffset
				}).appendTo("body").fadeIn(200);
        }
    }


    var previousPoint = null;
    $.plot(sel, sets, options);
    sel.bind("plothover", function (event, pos, item) {
				 if (item) {
					 if (previousPoint != item.datapoint) {
						 previousPoint = item.datapoint;
						 $("#tooltip").remove();
						 var x = item.datapoint[0];
						 var y = item.datapoint[1];
						 var text = "Run time: " + graph.runTimes[x] + "<br/>";
						 text += "Commit: " + graph.commits[x] + "<br/>";
						 showToolTip(item.pageX, item.pageY, text);
					 }
				 } else {
					 $("#tooltip").remove();
					 previousPoint = null;
				 }
			 });
}


function drawAllGraphs() {
	for(var i = 0; i < graphs.length; i++){
		var g = graphs[i];
		drawGraph(g);
	}
}

MochiKit.DOM.addLoadEvent(drawAllGraphs);
