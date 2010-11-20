// === Space3d ===
// Copyright Paul Clark (c) 2010



// Common {{{

if (!this.console) {
	console = {
		log: function(o) { }
	};
}

// A Fresh canvas is best
while (document.getElementsByTagName("canvas").length > 0) {
	var c = document.getElementsByTagName("canvas")[0];
	c.parentNode.removeChild(c);
}

var canvas = (
		document.getElementsByTagName("canvas").length>0
		&& document.getElementsByTagName("canvas")[0]
	) || document.body.appendChild(document.createElement("canvas"));

canvas.width = window.innerWidth*2/3;
canvas.height = window.innerHeight*2/3;

var ctx = canvas.getContext("2d");
var w = canvas.width;
var h = canvas.height;

var cenx = w/2, ceny = h/2;
var scalex = h/2, scaley = h/2;

var cube = {
	centre: [0,0,0],
	boundingSphereRadius: Math.sqrt(3),
	vertices: [
		[-1,-1,-1], [+1,-1,-1],
		[-1,+1,-1], [+1,+1,-1],
		[-1,-1,+1], [+1,-1,+1],
		[-1,+1,+1], [+1,+1,+1]
	],
	faces: [
		[0,1,3,2],
		[4,5,7,6],
		[2,3,7,6],
		[0,1,5,4],
		[1,3,7,5],
		[2,0,4,6]
	]
};

var cameraDepth = -2.2;
var camera = [0,0,cameraDepth];

function project(vector) {
	newVect = [];
	var depth = vector[2] - cameraDepth;
	// if (depth <= 0)
		// return [0,0,0];
	newVect[0] = cenx+scalex*vector[0]/depth;
	newVect[1] = ceny+scaley*vector[1]/depth;
	// newVect[0] += vector[2]*w/12;
	// newVect[1] -= vector[2]*w/20;
	console.log("project("+JSON.stringify(vector)+") -> "+JSON.stringify(newVect));
	return newVect;
}

// }}}



// Space3D {{{

var scene = {
	objects: [],
	ships: [],
	missiles: []
};

function Ship() {
	this.commandBlock = null;
	this.blocks = [];
}

function BlockView(block) {
	this.block = block;
	// Return stuff renderer needs to draw
}

function Block(color,shipPos) {
	this.color = color;
	this.positionRelativeToShip = shipPos;
	this.rotationRelativeToShip = [0,0,0];
	this.view = new BlockView(this);
	// The index of the corner is more interesting than the corner itself, since
	// we may want to use the cached post-transform vertex vector instead.
	// this.polys = cube.faces.map( function(p) {
		// return p.map( function(v) {
			// return vertices[v];
		// });
	// });
	this.polys = cube.faces.map( function(p) { return p; } );
}

function buildShip() {
	var ship = new Ship();
	ship.commandBlock = new Block("green",[0,0,0]);
	for (var x=-1;x<=+1;x++) {
		for (var y=-1;y<=+1;y++) {
			for (var z=-1;z<=+1;z++) {
				if (x==0 && y==0 && z==0)
					continue;
				if (Math.random() > 0.7)
					continue;
				var block = new Block(360*Math.random(),[3*x,3*y,3*z]);
				ship.blocks.push(block);
				block.verticesInShipSpace = cube.vertices.map(
					function(v) {
						return V3.add(v,block.positionRelativeToShip);
					}
				);
			}
		}
	}
	ship.position = [2.0*(Math.random()-0.5),2.0*(Math.random()-0.5),5.0+4.0*Math.random()];
	ship.yaw = Math.random();
	ship.pitch = Math.random();
	ship.roll = Math.random();
	return ship;
}

scene.ships.push(buildShip());

function furthestFirst(p,q) {
	return q.transformedCentre[2] - p.transformedCentre[2];
}

function UnsortedTree(sortfn) {
	this.item = null;
	this.left = null;
	this.right = null;
	this.addItem = function(item) {
		if (this.item == null) {
			this.item = item;
		} else {
			if (sortfn(item,this.item)>0) {
				if (this.right == null)
					this.right = new Tree(sortfn);
				this.right.addItem(item);
			} else {
				if (this.left == null)
					this.left = new Tree(sortfn);
				this.left.addItem(item);
			}
		}
	};
	this.iterateItems = function(callback) {
		if (left)
			left.iterateItems(callback);
		if (item)
			callback(item);
		if (right)
			right.iterateItems(callback);
	}
}

Tree = UnsortedTree;

function frontToBack(shipA,shipB) {
	return M4x4.mul(shipA.position,world2camera)[2] > M4x4.mul(shipB.position,world2camera)[2];
}

function updateWorld(scene) {
	// for (var ship in scene.ships) {
	for (var i=0;i<scene.ships.length;i++) {
		var ship = scene.ships[i];
		ship.yaw += Math.PI/131;
		ship.roll += Math.PI/87;
		ship.pitch += Math.PI/159;
	}
}

function render(scene) {

	updateWorld(scene);

	ctx.globalAlpha = 1.0;
	ctx.strokeStyle = 'rgba(0,0,0,0)';
	ctx.lineWidth = 4.0;
	ctx.fillStyle = 'black';
	ctx.fillRect(0,0,w,h);

	// Ships live in world space
	// But we want to sort them according to camera space

	// ships.sort(furthestFirst);

	var vertices = [];

	// Transformations:
	// - block to ship coords (many, but rarely change)
	// - ship coords to world coords (constantly changing)
	// - world coords to camera coords (sometimes changes)

	var tree = new Tree(frontToBack);

	function plotPolygon(block,polygon) {
		var firstVertexScr = block.transformedVertices[polygon[0]];
		ctx.moveTo(firstVertexScr[0],firstVertexScr[1]);
		for (var j=1;j<polygon.length;j++) {
			var vertexScr = block.transformedVertices[polygon[j]];
			ctx.lineTo(vertexScr[0],vertexScr[1]);
		}
		ctx.lineTo(firstVertexScr[0],firstVertexScr[1]);
	}

	ctx.globalAlpha = 0.5;

	console.log("About to render "+scene.ships.length+" ships.");

	function M4x4_apply(matrix,vector) {
		var r = [];
		 r[0] =
			  vector[0] * matrix[0] +
			  vector[0+1] * matrix[4] +
			  vector[0+2] * matrix[8] +
			  matrix[12];
		 r[0+1] =
			  vector[0] * matrix[1] +
			  vector[0+1] * matrix[5] +
			  vector[0+2] * matrix[9] +
			  matrix[13];
		 r[0+2] =
			  vector[0] * matrix[2] +
			  vector[0+1] * matrix[6] +
			  vector[0+2] * matrix[10] +
			  matrix[14];
		return r;
	}

	for (var i=0;i<scene.ships.length;i++) {
		var ship = scene.ships[i];
		// console.log("Rendering ship "+ship);
		var pitchTransform = M4x4.makeRotate(ship.pitch,[1,0,0]);
		// console.log("pitchTransform="+JSON.stringify(pitchTransform));
		var yawTransform = M4x4.makeRotate(ship.yaw,[0,1,0]);
		// var translation = M4x4.makeTranslate3(ship.position[0],ship.position[1],ship.position[2]);
		// console.log("translation="+JSON.stringify(translation));
		var intermediateTransform = M4x4.mul(pitchTransform,yawTransform);
		// var translation = M4x4.translate(ship.position,intermediateTransform);
		var translation = M4x4.makeTranslate(ship.position);
		ship.transform = M4x4.mul(translation,intermediateTransform);
		// ship.transform = translation;
		// console.log("ship.transform="+JSON.stringify(ship.transform));
		var unTransform = M4x4.inverseOrthonormal(ship.transform);
		var cameraRelativeToShip = M4x4_apply(unTransform,camera);
		ship.blocks.forEach(function(block){
			block.cachedDistanceFromCamera = V3.distance(block.positionRelativeToShip,cameraRelativeToShip);
		});
		function distanceFromCamera(p,q) {
			// return V3.length(V3.sub(p.positionRelativeToShip,cameraRelativeToShip)) < V3.length(V3.sub(q.positionRelativeToShip,cameraRelativeToShip));
			return p.cachedDistanceFromCamera < q.cachedDistanceFromCamera;
		}
		ship.blocks.sort(distanceFromCamera);
		for (var j=0;j<ship.blocks.length;j++) {
			var block = ship.blocks[j];
			block.transformedVertices = block.verticesInShipSpace.map( function(v) {
					// console.log("Input: "+JSON.stringify(ship.transform)+" "+JSON.stringify(v)+" Middle: "+ JSON.stringify(M4x4_apply(ship.transform,v)) +" Output: "+project(M4x4_apply(ship.transform,v)));
					return project(M4x4_apply(ship.transform,v));
			}); // now in world space
			// console.log("Transformed "+block.transformedVertices.length+" vertices for block "+block);
			ctx.globalAlpha = 0.5;
			ctx.strokeStyle = "hsla("+block.color+",100%,80%,1.0)"
			ctx.fillStyle = "hsla("+block.color+",50%,50%,1.0)"
			if (Math.random()<0.01) {
				ctx.globalAlpha = 0.8;
				ctx.fillStyle = "white";
			}
			for (var k=0;k<block.polys.length;k++) {
				var poly = block.polys[k];
				// transform polygon to ship space (didn't need to)
				// transform vertices from ship space to world space (done)
				// transform polygon from ship space to world space (we did vertices)
				// transform from world space to camera space
				// add into the octree
				ctx.beginPath();
				plotPolygon(block,poly);
				// ctx.strokeStyle = block.color;
				// ctx.fillStyle = block.color;
				ctx.fill();
				ctx.stroke();
			}
		}
	}

}

// }}}



// Animator {{{

(function(){

	function drawFrame() {

		render(scene);

		frameCount++;
		var timeRunning = (new Date().getTime() - startTime) / 1000;
		var framerate = Math.round(frameCount / timeRunning);
		ctx.globalAlpha = 1.0;
		ctx.fillStyle = 'white';
		ctx.fillText(framerate+" fps",40,10);

	}

	var frameCount = 0;
	var startTime = new Date().getTime();

	var animation = setInterval(drawFrame,1000/20);

	// document.body.mouseover/out works great in Chromium, except it fails to fire for Alt-Tab.
	/* document.body.addEventListener('mouseover',function(e){
		console.log("mouseover fired on "+this);
		if (!animation) {
			animation = setInterval(drawFrame,50);
		}
	},true); */

	// Firefox doesn't fire anything on document.body so we use canvas:
	canvas.addEventListener('mouseout',function(e){
		console.log("mouseout fired on "+this);
		if (animation) {
			clearInterval(animation);
			animation=null;
		}
	},true);

})();

// }}}

