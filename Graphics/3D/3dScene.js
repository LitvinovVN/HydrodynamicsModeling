import * as THREE from 'https://unpkg.com/three/build/three.module.js';

			const scene = new THREE.Scene();
			scene.background = new THREE.Color(0xffffff);
			const camera = new THREE.PerspectiveCamera( 75, window.innerWidth / window.innerHeight, 0.1, 1000 );

			const renderer = new THREE.WebGLRenderer();
			renderer.setSize( window.innerWidth, window.innerHeight );
			document.body.appendChild( renderer.domElement );

			const geometry = new THREE.BoxGeometry();
			const material = new THREE.MeshBasicMaterial( { color: 0x0055ff } );
			const cube = new THREE.Mesh( geometry, material );
			cube.position.x = -1
			cube.position.y = -1
			cube.position.z = -1
			scene.add( cube );

			////////
			//create a blue LineBasicMaterial
			const materialAxis = new THREE.LineBasicMaterial( { color: 0x0000ff } );
			const pointsAxisX = [];
			pointsAxisX.push( new THREE.Vector3( -3, 0, 0 ) );
			pointsAxisX.push( new THREE.Vector3( 3, 0, 0 ) );
			const geometryAxisX = new THREE.BufferGeometry().setFromPoints( pointsAxisX );
			const lineAxisX = new THREE.Line( geometryAxisX, materialAxis );
			scene.add( lineAxisX );
			
			const pointsAxisY = [];
			pointsAxisY.push( new THREE.Vector3( 0, -3, 0 ) );
			pointsAxisY.push( new THREE.Vector3( 0, 3, 0 ) );
			const geometryAxisY = new THREE.BufferGeometry().setFromPoints( pointsAxisY );
			const lineAxisY = new THREE.Line( geometryAxisY, materialAxis );
			scene.add( lineAxisY );

			const pointsAxisZ = [];
			pointsAxisZ.push( new THREE.Vector3( 0, 0, -3 ) );
			pointsAxisZ.push( new THREE.Vector3( 0, 0, 3 ) );
			const geometryAxisZ = new THREE.BufferGeometry().setFromPoints( pointsAxisZ );
			const lineAxisZ = new THREE.Line( geometryAxisZ, materialAxis );
			scene.add( lineAxisZ );
			////////

			var geometrySphere = new THREE.SphereGeometry(0.05, 32, 32); 
			var materialSphere = new THREE.MeshBasicMaterial( {color: 0x0fffff} );
			var meshSphere = new THREE.Mesh(geometrySphere, materialSphere);
			meshSphere.position.x = 0.2
			meshSphere.position.y = 0.2
			meshSphere.position.z = 0
			scene.add(meshSphere);

			for (var i = 0; i < 9; i++) {
				for (var j = 0; j < 9; j++) {					
					var meshSphere = new THREE.Mesh(geometrySphere, materialSphere);
					meshSphere.position.x = 0.2 * i
					meshSphere.position.y = 0.2 * j
					meshSphere.position.z = 0
					scene.add(meshSphere);
				}
			}

			for (var i = 0; i < 9; i++) {
				for (var j = 0; j < 9; j++) {					
					var meshSphere = new THREE.Mesh(geometrySphere, materialSphere);
					meshSphere.position.x = 0.2 * i
					meshSphere.position.y = 0.2 * j
					meshSphere.position.z = 0.5
					scene.add(meshSphere);
				}
			}

			////////


			camera.position.z = 5;

			const animate = function () {
				requestAnimationFrame( animate );

				scene.rotation.x += 0.005;
				scene.rotation.y += 0.001;

				//cube.rotation.x += 0.005;
				//cube.rotation.y += 0.005;

				//lineAxisX.rotation.x += 0.005;
				//lineAxisX.rotation.y += 0.005;

				//lineAxisY.rotation.x += 0.005;
				//lineAxisY.rotation.y += 0.005;

				//lineAxisZ.rotation.x += 0.005;
				//lineAxisZ.rotation.y += 0.005;

				//meshSphere.rotation.x += 0.005;
				//meshSphere.rotation.y += 0.005;

				renderer.render( scene, camera );
			};

			animate();

			/*const controls = new THREE.OrbitControls( camera, renderer.domElement );

			//controls.update() must be called after any manual changes to the camera's transform
			camera.position.set( 0, 20, 100 );
			controls.update();

			function animate() {

				requestAnimationFrame( animate );

				// required if controls.enableDamping or controls.autoRotate are set to true
				controls.update();

				renderer.render( scene, camera );

			}*/