(function(t){function e(e){for(var r,s,h=e[0],o=e[1],l=e[2],c=0,f=[];c<h.length;c++)s=h[c],n[s]&&f.push(n[s][0]),n[s]=0;for(r in o)Object.prototype.hasOwnProperty.call(o,r)&&(t[r]=o[r]);u&&u(e);while(f.length)f.shift()();return a.push.apply(a,l||[]),i()}function i(){for(var t,e=0;e<a.length;e++){for(var i=a[e],r=!0,h=1;h<i.length;h++){var o=i[h];0!==n[o]&&(r=!1)}r&&(a.splice(e--,1),t=s(s.s=i[0]))}return t}var r={},n={index:0},a=[];function s(e){if(r[e])return r[e].exports;var i=r[e]={i:e,l:!1,exports:{}};return t[e].call(i.exports,i,i.exports,s),i.l=!0,i.exports}s.m=t,s.c=r,s.d=function(t,e,i){s.o(t,e)||Object.defineProperty(t,e,{enumerable:!0,get:i})},s.r=function(t){"undefined"!==typeof Symbol&&Symbol.toStringTag&&Object.defineProperty(t,Symbol.toStringTag,{value:"Module"}),Object.defineProperty(t,"__esModule",{value:!0})},s.t=function(t,e){if(1&e&&(t=s(t)),8&e)return t;if(4&e&&"object"===typeof t&&t&&t.__esModule)return t;var i=Object.create(null);if(s.r(i),Object.defineProperty(i,"default",{enumerable:!0,value:t}),2&e&&"string"!=typeof t)for(var r in t)s.d(i,r,function(e){return t[e]}.bind(null,r));return i},s.n=function(t){var e=t&&t.__esModule?function(){return t["default"]}:function(){return t};return s.d(e,"a",e),e},s.o=function(t,e){return Object.prototype.hasOwnProperty.call(t,e)},s.p="/";var h=window["webpackJsonp"]=window["webpackJsonp"]||[],o=h.push.bind(h);h.push=e,h=h.slice();for(var l=0;l<h.length;l++)e(h[l]);var u=o;a.push([0,"chunk-vendors"]),i()})({0:function(t,e,i){t.exports=i("56d7")},"034f":function(t,e,i){"use strict";var r=i("64a9"),n=i.n(r);n.a},"56d7":function(t,e,i){"use strict";i.r(e);i("cadf"),i("551c"),i("f751"),i("097d");var r=i("2b0e"),n=function(){var t=this,e=t.$createElement,i=t._self._c||e;return i("b-container",{attrs:{fluid:"",id:"app"}},[i("b-row",[i("b-col",{attrs:{cols:"2"}},[i("c-sidebar",{ref:"sidebar"})],1),i("b-col",{attrs:{cols:"10"}},[i("canvas",{ref:"canvas",attrs:{width:"800",height:"600"}}),i("div",{staticClass:"float-right"},[i("c-debug-panel",{on:{valueChanged:function(e){return t.debugValueChanged(e)}}}),i("c-information")],1)])],1)],1)},a=[],s=i("59ad"),h=i.n(s),o=function(){var t=this,e=t.$createElement,i=t._self._c||e;return i("div",[i("b-card",{attrs:{header:"相机设置"}},[i("div",[t._v("摄像机θ角(与Z轴正半轴夹角): "+t._s(t.cameraTheta.toFixed(1))+"\r\n      "),i("b-form-input",{attrs:{type:"range",id:"cameraTheta",value:t.cameraTheta,min:"0",max:"180"},on:{change:function(e){return t.cameraThetaChanged(e)}}})],1),i("div",[t._v("摄像机φ角(与X轴正半轴夹角): "+t._s(t.cameraPhi.toFixed(1))+"\r\n      "),i("b-form-input",{attrs:{type:"range",id:"cameraPhi",value:t.cameraPhi,min:"0",max:"360"},on:{change:function(e){return t.cameraPhiChanged(e)}}})],1),i("div",[t._v("摄像机距原点: "+t._s(t.cameraDis)+"\r\n      "),i("b-form-input",{attrs:{type:"range",id:"cameraDis",value:t.cameraDis,min:"1",max:"180"},on:{change:function(e){return t.cameraDisChanged(e)}}})],1)]),i("b-card",{attrs:{header:"球体参数设置"}},[i("div",[t._v("参考球半径: "+t._s(t.sphere.size)+"\r\n      "),i("b-form-input",{attrs:{type:"range",id:"sphereRadius",value:t.sphere.size,min:"1",max:"10"},on:{change:function(e){return t.sphereRadiusChanged(e)}}})],1),i("b-form-group",{attrs:{label:"绘制球体的模式"}},[i("b-radio-group",{staticClass:"sphereMode",model:{value:t.sphere.drawMode,callback:function(e){t.$set(t.sphere,"drawMode",e)},expression:"sphere.drawMode"}},[i("div",[i("b-radio",{attrs:{value:0}},[t._v("Surface")])],1),i("div",[i("b-radio",{attrs:{value:1}},[t._v("Line")])],1),i("div",[i("b-radio",{attrs:{value:2}},[t._v("lessLine")])],1)])],1)],1),i("b-card",{attrs:{header:"其他参数设置"}},[i("div",{staticClass:"option"},[i("b-checkbox",[t._v("绘制路径")]),i("b-checkbox",[t._v("绘制模拟器")]),i("b-checkbox",[t._v("球坐标系")]),i("b-checkbox",[t._v("显示修正圆圈")])],1),i("div",{staticClass:"action"},[i("b-btn",[t._v("重置摄像机位置")]),i("b-btn",[t._v("重置路径")]),i("b-btn",[t._v("打点")]),i("b-btn",[t._v("重置打点")])],1),i("div",[t._v("球面透明度: "+t._s(t.sphereAlpha)+"\r\n      "),i("b-form-input",{attrs:{type:"range",id:"sphereOpacity",value:t.sphereAlpha,min:"0",max:"100"},on:{change:function(e){t.sphere.alpha=parseFloat(e)/100}}})],1),i("div",[t._v("指示器大小: "+t._s(t.indicatorSize.toFixed(1))+"\r\n      "),i("b-form-input",{attrs:{type:"range",id:"indicatorSize",value:t.indicatorSize,min:"1",max:"100"},on:{change:function(e){return t.setIndicatorSize(e)}}})],1),i("div",[t._v("轨迹宽度 "+t._s(t.pathWidth)+"\r\n      "),i("b-form-input",{attrs:{type:"range",id:"pathWidth",value:t.pathWidth,min:"1",max:"100"},on:{change:function(e){return t.setPathWidth(e)}}})],1),i("div",{staticClass:"hidden"},[t._v("模拟器大小\r\n      "),i("b-form-input",{attrs:{type:"range",id:"pathWidth",value:t.pathWidth,min:"1",max:"100"},on:{change:function(e){return t.setPathWidth(e)}}})],1)])],1)},l=[];function u(t){return 180*t/Math.PI}var c={data:function(){return{camera:{theta:0,phi:0,dis:4},sphere:{size:4,alpha:1,drawMode:0},indicator:{size:1},sensorPath:{size:1}}},computed:{cameraPhi:function(){return u(this.camera.phi)},cameraTheta:function(){return u(this.camera.theta)},cameraDis:function(){return this.camera.dis},sphereAlpha:function(){return 100*this.sphere.alpha},indicatorSize:function(){return 100*this.indicator.size},pathWidth:function(){return this.sensorPath.size}},methods:{cameraPhiChanged:function(t){this.camera.rotate(null,t,null)},cameraThetaChanged:function(t){this.camera.rotate(t,null,null)},cameraDisChanged:function(t){this.camera.rotate(null,null,t)},sphereRadiusChanged:function(t){this.sphere.setSize(t)},setIndicatorSize:function(t){this.indicator.setScale(h()(t)/100)},setPathWidth:function(t){this.sensorPath.setSize(t)},init:function(t){this.spacePath=t,console.log(t),this.camera=t.camera,this.sphere=t.sphere,this.indicator=t.sensorPoint,this.sensorPath=t.sensorPath,console.log(this.sphere)}},mounted:function(){console.log("Sidebar Mounted")}},f=c,v=i("2877"),d=Object(v["a"])(f,o,l,!1,null,null,null),g=d.exports,p=function(){var t=this,e=t.$createElement;t._self._c;return t._m(0)},m=[function(){var t=this,e=t.$createElement,i=t._self._c||e;return i("div",[i("div",[t._v("Picth")]),i("div",[t._v("Roll")]),i("div",[t._v("Heading")])])}],x={},_=x,b=Object(v["a"])(_,p,m,!1,null,null,null),A=b.exports,M=function(){var t=this,e=t.$createElement,i=t._self._c||e;return i("div",[i("div",[t._v("Heading\n    "),i("b-form-input",{attrs:{type:"range",id:"heading",min:"0",max:"360"},on:{change:function(e){return t.$emit("valueChanged",{key:"heading",value:t.heading})}},model:{value:t.heading,callback:function(e){t.heading=e},expression:"heading"}})],1),i("div",[t._v("Picth\n    "),i("b-form-input",{attrs:{type:"range",id:"pitch",min:"-90",max:"90"},on:{change:function(e){return t.$emit("valueChanged",{key:"pitch",value:t.pitch})}},model:{value:t.pitch,callback:function(e){t.pitch=e},expression:"pitch"}})],1),i("div",[t._v("Roll\n    "),i("b-form-input",{attrs:{type:"range",id:"roll",min:"0",max:"180"},on:{change:function(e){return t.$emit("valueChanged",{key:"roll",value:t.roll})}},model:{value:t.roll,callback:function(e){t.roll=e},expression:"roll"}})],1)])},y=[],E={data:function(){return{heading:0,pitch:0,roll:0}}},R=E,S=Object(v["a"])(R,M,y,!1,null,null,null),C=S.exports,T=i("d225"),P=i("b0b4"),L={gl:"not initialized"},F={},B={},O=(i("af56"),i("63d9"),i("308d")),k=i("6bb5"),w=i("4e2b"),I=function(){function t(e){Object(T["a"])(this,t),void 0===e&&(e={}),this.sides=e.sides||24,this.alpha=e.alpha||1,this.color=e.color||[.8,.8,.8],this.size=e.size||1,this.gl=e.gl,this.buffers={},this.mMatrix=mat4.create(),this.pvMatrix=mat4.create(),this.mvpMatrix=mat4.create(),this.quat=quat.create(),this.vscale=vec3.fromValues(1,1,1),this.translation=vec3.create(),this.visible=!0,this._x=0,this._y=0,this._z=0,this._rotateX=0,this._rotateY=0,this._rotateZ=0}return Object(P["a"])(t,[{key:"createArrayBuffer",value:function(t,e,i){var r=L.gl.createBuffer();return void 0===i&&(i=L.gl.ELEMENT_ARRAY_BUFFER,r.itemSize=t.itemSize||1,t instanceof Float32Array&&(i=L.gl.ARRAY_BUFFER,r.itemSize=t.itemSize||3),r.numItems=t.length/r.itemSize),L.gl.bindBuffer(i,r),L.gl.bufferData(i,t,e),L.gl.bindBuffer(i,null),r}},{key:"subBuffer",value:function(t,e,i){var r=L.gl.ELEMENT_ARRAY_BUFFER;i instanceof Float32Array&&(r=L.gl.ARRAY_BUFFER),L.gl.bindBuffer(r,t),L.gl.bufferSubData(r,e,i)}}]),t}();function N(t,e,i){var r=U(t,e),n=U(t,e,0,2*Math.PI,i),a=r.concat(n),s=[],h=0;for(h=1;h<e;h++)s.push(0,h+1,h);for(s.push(0,1,h),h=e+2;h<2*e+1;h++)s.push(e+1,h,h+1);for(s.push(e+1,h,e+2),h=1;h<e;h++)s.push(e+1+h,h,h+1),s.push(e+h+1,h+1,e+h+2);return s.push(e+1+h,h,1),s.push(e+h+1,1,e+2),{vertex:a,index:s}}function U(t,e,i,r,n){var a=[],s=0,h=0,o=n||0,l=i||0,u=r||2*Math.PI,c=(u-l)/e;a=a.concat([s,h,0]);for(var f=0;f<e;f++)s=Math.cos(f*c+l)*t,h=Math.sin(f*c+l)*t,a=a.concat([s,h,o]);return a}function D(t,e,i,r){for(var n=vec3.create(),a=r||0,s=0;s<t.length;s+=3)vec3.set(n,t[s+0],t[s+1],t[s+2]+a),vec3.rotateY(n,n,[0,0,0],e),vec3.rotateZ(n,n,[0,0,0],i),t[s+0]=n[0],t[s+1]=n[1],t[s+2]=n[2]}function j(t){return t*Math.PI/180}function z(t,e,i){var r=[],n=0;if(e.length<=3)if(void 0===i)for(n=0;n<t.length;n+=3)r=r.concat(t[n],t[n+1],t[n+2]),r=r.concat(e[0],e[1],e[2]);else if(i.length<=3)for(n=0;n<t.length;n+=3)r=r.concat(t[n],t[n+1],t[n+2]),r=r.concat(e[0],e[1],e[2]),r=r.concat(i[0],i[1],i[2]);else for(n=0;n<t.length;n+=3)r=r.concat(t[n],t[n+1],t[n+2]),r=r.concat(e[0],e[1],e[2]),r=r.concat(i[n],i[n+1],i[n+2]);else if(void 0===i)for(n=0;n<t.length;n+=3)r=r.concat(t[n],t[n+1],t[n+2]),r=r.concat(e[n],e[n+1],e[n+2]);else if(i.length<=3)for(n=0;n<t.length;n+=3)r=r.concat(t[n],t[n+1],t[n+2]),r=r.concat(e[n],e[n+1],e[n+2]),r=r.concat(i[0],i[1],i[2]);else for(n=0;n<t.length;n+=3)r=r.concat(t[n],t[n+1],t[n+2]),r=r.concat(e[n],e[n+1],e[n+2]),r=r.concat(i[n],i[n+1],i[n+2]);return r}function V(t,e,i){var r=Math.sin(t),n=Math.cos(t),a=Math.sin(e),s=Math.cos(e),h=i*r*s,o=i*r*a,l=i*n;return vec3.fromValues(h,o,l)}var Y=function(t){function e(t){var i;return Object(T["a"])(this,e),i=Object(O["a"])(this,Object(k["a"])(e).call(this,t)),i.type="Coord",i.length=10,i.init(),i}return Object(w["a"])(e,t),Object(P["a"])(e,[{key:"init",value:function(){var t=N(.01,this.sides,this.length);D(t.vertex,j(90),0);var e=z(t.vertex,[.9,.1,0],[1,1,1]),i=N(.01,this.sides,this.length);D(i.vertex,j(90),j(90));var r=z(i.vertex,[0,.9,0],[1,1,1]),n=N(.01,this.sides,this.length),a=z(n.vertex,[0,0,.9],[1,1,1]),s=[];s=s.concat(e),s=s.concat(r),s=s.concat(a);var h=[];h=h.concat(t.index);for(var o=2*this.sides+2,l=0;l<i.index.length;l++)h.push(o+i.index[l]);o=4*this.sides+4;for(l=0;l<n.index.length;l++)h.push(o+n.index[l]);this.buffers.vertex=this.createArrayBuffer(new Float32Array(s),L.gl.STATIC_DRAW),this.buffers.index=this.createArrayBuffer(new Uint16Array(h),L.gl.STATIC_DRAW),this.buffers.index.numItems=h.length}},{key:"paint",value:function(){L.gl.uniform1f(B.alpha,this.alpha),L.gl.uniformMatrix4fv(B.m_matrix,!1,this.mMatrix),L.gl.uniformMatrix4fv(B.pmv_matrix,!1,this.mvpMatrix),L.gl.bindBuffer(L.gl.ARRAY_BUFFER,this.buffers.vertex),L.gl.vertexAttribPointer(F.vertex_position,3,L.gl.FLOAT,!1,36,0),L.gl.vertexAttribPointer(F.color,3,L.gl.FLOAT,!1,36,12),L.gl.vertexAttribPointer(F.vertex_normal,3,L.gl.FLOAT,!1,36,24),L.gl.bindBuffer(L.gl.ELEMENT_ARRAY_BUFFER,this.buffers.index),L.gl.drawElements(L.gl.TRIANGLES,this.buffers.index.numItems,L.gl.UNSIGNED_SHORT,0)}},{key:"onViewChanged",value:function(t){this.pvMatrix=t||this.pvMatrix,mat4.mul(this.mvpMatrix,this.pvMatrix,this.mMatrix)}}]),e}(I),W=Y,G=function(){function t(e){Object(T["a"])(this,t),this.defaultTheta=45,this.defaultPhi=45,this.defaultDis=15,this.defaultViewWidth=800,this.defaultViewHeight=600,this.type="Camera",this.theta=e.theta||this.defaultTheta,this.theta=j(this.theta),this.phi=e.phi||this.defaultPhi,this.phi=j(this.phi),this.dis=e.dis||this.defaultDis,this.width=e.width||this.defaultViewWidth,this.height=e.height||this.defaultViewHeight,this.pos=V(this.theta,this.phi,this.dis),this.up=vec3.fromValues(0,0,1),this.lookat=vec3.fromValues(0,0,0),this.pMatrix=mat4.create(),this.vMatrix=mat4.create(),this.pvMatrix=mat4.create(),this.setSize(this.width,this.height)}return Object(P["a"])(t,[{key:"update",value:function(){mat4.lookAt(this.vMatrix,this.pos,this.lookat,this.up),mat4.mul(this.pvMatrix,this.pMatrix,this.vMatrix),void 0!==this.watcher&&this.watcher(this.pvMatrix)}},{key:"recv",value:function(t){this.watcher=t}},{key:"rotate",value:function(t,e,i){t&&(t<.01?t=.01:t>179.99&&(t=179.99),this.theta=j(t)),e&&(this.phi=j(e)),i&&(this.dis=i),this.pos=V(this.theta,this.phi,this.dis),this.update()}},{key:"setSize",value:function(t,e){this.width=t,this.height=e,L.gl.viewport(0,0,t,e),mat4.perspective(this.pMatrix,.25*Math.PI,t/e,.5,500),this.update()}},{key:"reset",value:function(){this.rotate(this.defaultTheta,this.defaultPhi,this.dis),this.update()}}]),t}(),H=function(){function t(e){Object(T["a"])(this,t);var i=L.gl;i.enable(i.DEPTH_TEST),i.depthFunc(i.LESS),i.enable(i.CULL_FACE),i.cullFace(i.BACK),i.clearColor(.97,.97,.97,1),i.clearDepth(1),i.pixelStorei(i.UNPACK_FLIP_Y_WEBGL,!1),i.enable(i.BLEND),i.blendFunc(i.SRC_ALPHA,i.ONE_MINUS_SRC_ALPHA),this.type="Scene",this.objs=[],this.light_direct=[.35,.35,.7]}return Object(P["a"])(t,[{key:"add",value:function(t,e){void 0!==t?(e?this.objs.unshift(t):this.objs.push(t),void 0!==this.camera&&this.camera.update()):console.log("[Warn] Undefined on Scene adding.")}},{key:"addCamera",value:function(t){var e=this;this.camera=t,t.recv(function(t){for(var i=0;i<e.objs.length;i++)e.objs[i].onViewChanged(t)})}},{key:"render",value:function(){this.running=!0;var t=this;function e(){L.gl.clear(L.gl.COLOR_BUFFER_BIT|L.gl.DEPTH_BUFFER_BIT);for(var i=0;i<t.objs.length;i++)t.objs[i].paint();t.running&&requestAnimationFrame(e)}requestAnimationFrame(e)}},{key:"getShader",value:function(t,e,i){var r=t.createShader(i);return t.shaderSource(r,e),t.compileShader(r),t.getShaderParameter(r,t.COMPILE_STATUS)?r:(console.log("JS:Shader compile failed"),console.log(t.getShaderInfoLog(r)),null)}},{key:"initShaders",value:function(t,e){var i=L.gl.createProgram(),r=this.getShader(L.gl,t,L.gl.VERTEX_SHADER);L.gl.attachShader(i,r);var n=this.getShader(L.gl,e,L.gl.FRAGMENT_SHADER);L.gl.attachShader(i,n),this._onShaderReady(i)}},{key:"_onShaderReady",value:function(t){L.gl.linkProgram(t),L.gl.useProgram(t),F.vertex_position=L.gl.getAttribLocation(t,"aVertexPosition"),L.gl.enableVertexAttribArray(F.vertex_position),F.vertex_normal=L.gl.getAttribLocation(t,"aVertexNormal"),L.gl.enableVertexAttribArray(F.vertex_normal),F.color=L.gl.getAttribLocation(t,"aColor"),L.gl.enableVertexAttribArray(F.color),F.texture=L.gl.getAttribLocation(t,"aTexture"),F.color=L.gl.getAttribLocation(t,"aColor"),B.pmv_matrix=L.gl.getUniformLocation(t,"uPMVMatrix"),B.m_matrix=L.gl.getUniformLocation(t,"uMMatrix"),B.n_matrix=L.gl.getUniformLocation(t,"uNMatrix"),B.alpha=L.gl.getUniformLocation(t,"uAlpha"),B.frag_color=L.gl.getUniformLocation(t,"uFragColor"),B.has_texture=L.gl.getUniformLocation(t,"uHasTexture"),B.light_direction=L.gl.getUniformLocation(t,"uLightDirection"),B.specColor=L.gl.getUniformLocation(t,"uSpecColor"),B.vertexColor=L.gl.getUniformLocation(t,"uVertColor"),L.gl.uniformMatrix4fv(B.n_matrix,!1,mat4.create()),L.gl.uniform3fv(B.light_direction,this.light_direct)}}]),t}(),q=function(t){function e(t){var i;return Object(T["a"])(this,e),i=Object(O["a"])(this,Object(k["a"])(e).call(this,t)),i.type="SensorPoint",i.inv_color=t.inv_color||[0,1,0],i.dis=4,i.pitch=0,i.heading=0,i.headingOffset=0,i.roll=0,i.callbacks=[],i.init(),i}return Object(w["a"])(e,t),Object(P["a"])(e,[{key:"init",value:function(){var t=N(this.size,this.sides,.01),e=t.vertex,i=t.index,r=0,n=[];for(r=0;r<=this.sides;r++)n=n.concat(this.inv_color);for(r=0;r<=this.sides;r++)n=n.concat(this.color);var a=z(e,n,[0,1,1]);this.index_count=i.length,this.buffers.vertex=this.createArrayBuffer(new Float32Array(a),L.gl.STATIC_DRAW),this.buffers.index=this.createArrayBuffer(new Uint16Array(i),L.gl.STATIC_DRAW)}},{key:"paint",value:function(){L.gl.bindBuffer(L.gl.ARRAY_BUFFER,this.buffers.vertex),L.gl.vertexAttribPointer(F.vertex_position,3,L.gl.FLOAT,!1,36,0),L.gl.vertexAttribPointer(F.color,3,L.gl.FLOAT,!1,36,12),L.gl.vertexAttribPointer(F.vertex_normal,3,L.gl.FLOAT,!1,36,24),L.gl.uniform1f(B.alpha,this.alpha),L.gl.uniformMatrix4fv(B.m_matrix,!1,this.mMatrix),L.gl.uniformMatrix4fv(B.pmv_matrix,!1,this.mvpMatrix),L.gl.bindBuffer(L.gl.ELEMENT_ARRAY_BUFFER,this.buffers.index),L.gl.drawElements(L.gl.TRIANGLES,this.index_count,L.gl.UNSIGNED_SHORT,0)}},{key:"onViewChanged",value:function(t){this.pvMatrix=t||this.pvMatrix,mat4.mul(this.mvpMatrix,this.pvMatrix,this.mMatrix)}},{key:"setScale",value:function(t){this.size=t,vec3.set(this.vscale,t,t,t),this.update()}},{key:"setTranslation",value:function(t,e,i){this.translation=V(j(e),j(i),t),quat.fromEuler(this.quat,0,e,i),this.update()}},{key:"setParam",value:function(t){this.dis=t.dis||this.dis,t.pitch&&(this.pitch=t.pitch),t.heading&&(this.heading=t.heading,t.heading=t.heading-this.headingOffset),t.roll&&(this.roll=t.roll);var e=90-this.pitch;this.setTranslation(this.dis,e,this.heading)}},{key:"update",value:function(){mat4.fromRotationTranslationScale(this.mMatrix,this.quat,this.translation,this.vscale),this.onViewChanged();for(var t=this.spherical(),e=0;e<this.callbacks.length;e++)this.callbacks[e]({dis:this.dis,pitch:this.pitch,heading:this.heading,roll:this.roll,theta:t[0],phi:t[1],size:this.size})}},{key:"addParamCallback",value:function(t){this.callbacks.push(t)}},{key:"spherical",value:function(){var t=(90-this.pitch)/180*Math.PI,e=this.heading/180*Math.PI;return vec3.fromValues(t,e,this.dis)}},{key:"reset",value:function(){this.headingOffset+=this.heading,this.setParam({pitch:this.pitch,heading:this.heading,roll:this.roll,dis:this.dis})}}]),e}(I),X=function(t){function e(t){var i;return Object(T["a"])(this,e),i=Object(O["a"])(this,Object(k["a"])(e).call(this,t)),i.MaxPathNum=57600,i.type="SensorPath",i.all_index_count=0,i.cur_path_count=0,i.cur_index_count=0,i.cur_pi=0,i.buffer_path_bytes=4*i.MaxPathNum,i.buffer_index_bytes=2*i.MaxPathNum,i.gap=1,i.pg=1,i.init(),i}return Object(w["a"])(e,t),Object(P["a"])(e,[{key:"init",value:function(){this.angle=this.last_point,this.path=[],this.index=[],this.buffers.path=[],this.buffers.index=[],this.createBuffer()}},{key:"paint",value:function(){if(this.visible){L.gl.uniform1f(B.alpha,this.alpha),L.gl.uniformMatrix4fv(B.m_matrix,!1,this.mMatrix),L.gl.uniformMatrix4fv(B.pmv_matrix,!1,this.mvpMatrix);for(var t=0;t<this.cur_pi;t++)L.gl.bindBuffer(L.gl.ARRAY_BUFFER,this.buffers.path[t]),L.gl.vertexAttribPointer(F.vertex_normal,3,L.gl.FLOAT,!1,24,0),L.gl.vertexAttribPointer(F.vertex_position,3,L.gl.FLOAT,!1,24,0),L.gl.vertexAttribPointer(F.color,3,L.gl.FLOAT,!1,24,12),L.gl.bindBuffer(L.gl.ELEMENT_ARRAY_BUFFER,this.buffers.index[t]),L.gl.drawElements(L.gl.TRIANGLES,this.MaxPathNum,L.gl.UNSIGNED_SHORT,0);this.cur_index_count>0&&(L.gl.bindBuffer(L.gl.ARRAY_BUFFER,this.buffers.path[this.cur_pi]),L.gl.vertexAttribPointer(F.vertex_normal,3,L.gl.FLOAT,!1,24,0),L.gl.vertexAttribPointer(F.vertex_position,3,L.gl.FLOAT,!1,24,0),L.gl.vertexAttribPointer(F.color,3,L.gl.FLOAT,!1,24,12),L.gl.bindBuffer(L.gl.ELEMENT_ARRAY_BUFFER,this.buffers.index[this.cur_pi]),L.gl.drawElements(L.gl.TRIANGLES,this.cur_index_count,L.gl.UNSIGNED_SHORT,0))}}},{key:"onViewChanged",value:function(t){this.pvMatrix=t||this.pvMatrix,mat4.mul(this.mvpMatrix,this.pvMatrix,this.mMatrix)}},{key:"updateBuffer",value:function(t){var e=[this.last_point.theta,this.last_point.phi,this.last_point.dis],i=this.getLinearPoint(t,e);this.all_index_count+=i.index.length,this.subBuffer(this.buffers.path[this.cur_pi],4*this.cur_path_count,new Float32Array(i.vertices)),this.subBuffer(this.buffers.index[this.cur_pi],2*this.cur_index_count,new Uint16Array(i.index)),this.cur_path_count+=i.vertices.length,this.cur_index_count+=i.index.length,this.cur_index_count>=this.MaxPathNum&&(this.cur_pi++,console.log("Info: [Path] create a new buffer!\n"),this.createBuffer(),this.resetCurrentPath())}},{key:"setGap",value:function(t){this.gap=t}},{key:"setSize",value:function(t){this.size=t}},{key:"getLinearPoint",value:function(t,e){var i,r=V(t[0],t[1],t[2]),n=V(e[0],e[1],e[2]),a=[r[0]-n[0],r[1]-n[1],r[2]-n[2]],s=V(.5*(t[0]+e[0]),.5*(t[1]+e[1]),t[2]),h=vec3.create();vec3.cross(h,a,s),vec3.normalize(h,h),vec3.scale(h,h,.02*this.size);var o=[],l=this,u=function(){o=o.concat(i),o=o.concat(l.color)};i=[r[0]-h[0],r[1]-h[1],r[2]-h[2]],u(),i=[r[0]+h[0],r[1]+h[1],r[2]+h[2]],u(),i=[n[0]-h[0],n[1]-h[1],n[2]-h[2]],u(),i=[n[0]+h[0],n[1]+h[1],n[2]+h[2]],u();var c=[],f=6,v=this.cur_path_count/f;return c.push(v+0,v+2,v+3,v+0,v+3,v+1),c.push(v+0,v+3,v+2,v+0,v+1,v+3),{vertices:o,index:c}}},{key:"onSphericalChanged",value:function(t){void 0===this.last_point&&(this.last_point=t);var e=V(this.last_point.theta,this.last_point.phi,this.last_point.dis),i=V(t.theta,t.phi,t.dis),r=vec3.dist(e,i),n=[t.theta,t.phi,t.dis];r>Math.PI*t.dis*.002&&(this.updateBuffer(n),this.angle=n,this.last_point=t)}},{key:"resetCurrentPath",value:function(t){this.cur_path_count=0,this.cur_index_count=0,this.pg=0}},{key:"resetAllPath",value:function(t){for(var e=1;e<=this.cur_pi;e++)gl.deleteBuffer(this.buffers.path[e]),gl.deleteBuffer(this.buffers.index[e]);this.all_index_count=0,this.cur_pi=0,this.last_point=void 0,this.resetCurrentPath()}},{key:"createBuffer",value:function(){this.buffers.path[this.cur_pi]=this.createArrayBuffer(this.buffer_path_bytes,L.gl.DYNAMIC_DRAW,L.gl.ARRAY_BUFFER),this.buffers.index[this.cur_pi]=this.createArrayBuffer(this.buffer_index_bytes,L.gl.DYNAMIC_DRAW,L.gl.ELEMENT_ARRAY_BUFFER)}}]),e}(I),$=function(t){function e(t){var i;return Object(T["a"])(this,e),i=Object(O["a"])(this,Object(k["a"])(e).call(this,t)),i.type="RecordPoint",i.max_vertex=1e3*i.sides,i.vertex_count=0,i.index_count=0,i.init(),i}return Object(w["a"])(e,t),Object(P["a"])(e,[{key:"init",value:function(){this.buffers.vertex=this.createArrayBuffer(4*this.max_vertex,L.gl.DYNAMIC_DRAW,L.gl.ARRAY_BUFFER),this.buffers.index=this.createArrayBuffer(2*this.max_vertex,L.gl.DYNAMIC_DRAW,L.gl.ELEMENT_ARRAY_BUFFER)}},{key:"paint",value:function(){0!==this.vertex_count&&this.visible&&(gl.bindBuffer(gl.ARRAY_BUFFER,this.buffers.vertex),gl.vertexAttribPointer(F.vertex_normal,3,gl.FLOAT,!1,24,0),gl.vertexAttribPointer(F.vertex_position,3,gl.FLOAT,!1,24,0),gl.vertexAttribPointer(F.color,3,gl.FLOAT,!1,24,12),gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,this.buffers.index),gl.uniform1f(B.alpha,this.alpha),gl.uniformMatrix4fv(B.m_matrix,!1,this.mMatrix),gl.uniformMatrix4fv(B.pmv_matrix,!1,this.mvpMatrix),gl.drawElements(gl.TRIANGLES,this.index_count,gl.UNSIGNED_SHORT,0))}},{key:"onViewChanged",value:function(t){this.pvMatrix=t||this.pvMatrix,mat4.mul(this.mvpMatrix,this.pvMatrix,this.mMatrix)}},{key:"setSize",value:function(t){this.size=t,vec3.fromValues(this.vscale,t,t,t),mat4.fromRotationTranslationScale(this.mMatrix,this.quat,this.translation,this.vscale),this.onViewChanged()}},{key:"record",value:function(){if(this.visible){console.log("[Info]: record a point.");var t=0,e=[],i=[];e=circleShape(this.size,this.sides,0,2*Math.PI),rotateVertex(e,this.theta,this.phi,this.dis);var r=z(e,[0,.5,.5]),n=this.vertex_count/6;for(t=1;t<this.sides;t++)i.push(n,n+t,n+t+1,n,n+t+1,n+t);i.push(n,n+this.sides,n+1,n,n+1,n+this.sides),this.subBuffer(this.buffers.vertex,4*this.vertex_count,new Float32Array(r)),this.subBuffer(this.buffers.index,2*this.index_count,new Uint16Array(i)),this.vertex_count+=r.length,this.index_count+=i.length}}},{key:"onSphericalChanged",value:function(t){this.theta=t.theta,this.phi=t.phi,this.dis=1.01*t.dis,this.size=.95*t.size}},{key:"reset",value:function(){this.vertex_count=0,this.index_count=0}}]),e}(I),Z=function(){function t(e){Object(T["a"])(this,t),PaintObj.call(this),this.type="Craft",this.url="qrc:/res/obj/craft.obj";var i=e.size||1;this.setScale(i),this.init()}return Object(P["a"])(t,[{key:"init",value:function(){var t=this;readFile(this.url,function(e){t.mesh=new ObjLoader.OBJ.Mesh(e),ObjLoader.OBJ.initMeshBuffers(gl,t.mesh)})}},{key:"paint",value:function(){void 0!==this.mesh&&this.visible&&(gl.uniform1f(uniforms.alpha,this.alpha),gl.uniform1i(uniforms.specColor,1),gl.uniform3fv(uniforms.vertexColor,this.color),gl.bindBuffer(gl.ARRAY_BUFFER,this.mesh.vertexBuffer),gl.vertexAttribPointer(attributes.vertex_position,this.mesh.vertexBuffer.itemSize,gl.FLOAT,!1,0,0),gl.bindBuffer(gl.ARRAY_BUFFER,this.mesh.normalBuffer),gl.vertexAttribPointer(attributes.vertex_normal,this.mesh.normalBuffer.itemSize,gl.FLOAT,!1,0,0),gl.uniformMatrix4fv(uniforms.m_matrix,!1,this.mMatrix),gl.uniformMatrix4fv(uniforms.pmv_matrix,!1,this.mvpMatrix),gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,this.mesh.indexBuffer),gl.drawElements(gl.TRIANGLES,this.mesh.indexBuffer.numItems,gl.UNSIGNED_SHORT,0),gl.uniform1i(uniforms.specColor,!1))}},{key:"onViewChanged",value:function(t){this.pvMatrix=t||this.pvMatrix,mat4.mul(this.mvpMatrix,this.pvMatrix,this.mMatrix)}},{key:"setRotation",value:function(t){this.pitch=t.pitch,this.heading=t.heading,this.roll=t.roll,mat4.fromScaling(this.mMatrix,this.vscale),mat4.rotateX(this.mMatrix,this.mMatrix,degToRad(90)),mat4.rotateY(this.mMatrix,this.mMatrix,degToRad(t.heading+270)),mat4.rotateX(this.mMatrix,this.mMatrix,degToRad(t.pitch)),mat4.rotateZ(this.mMatrix,this.mMatrix,degToRad(t.roll)),this.onViewChanged()}},{key:"setScale",value:function(t){this.size=t,vec3.set(this.vscale,t,t,t),sensorPoint.update()}}]),t}(),J=function(t){function e(t){var i;return Object(T["a"])(this,e),i=Object(O["a"])(this,Object(k["a"])(e).call(this,t)),i.MODE_SURFACE=0,i.MODE_LINE=1,i.MODE_LESSLINE=2,i.DEFAULT_RADIUS=4,i.type="Sphere",i.vn=t.vn||48,i.hn=.5*(t.hn||48),i.size=t.size||i.DEFAULT_RADIUS,i.drawMode=i.MODE_SURFACE,i.line_alpha=.65,i.alpha=.25,i.init(),i.setSize(),i}return Object(w["a"])(e,t),Object(P["a"])(e,[{key:"init",value:function(){var t=this.getVertex(),e=this.getIndex();this.vertexBuffer=this.createArrayBuffer(new Float32Array(t.vertices),L.gl.STATIC_DRAW),this.indexBuffer=this.createArrayBuffer(new Uint16Array(e.vertex_index),L.gl.STATIC_DRAW),this.lineIndexBuffer=this.createArrayBuffer(new Uint16Array(e.line_index),L.gl.STATIC_DRAW),this.lessLineIndexBuffer=this.createArrayBuffer(new Uint16Array(e.less_line_index),L.gl.STATIC_DRAW),this.lessLSIndexBuffer=this.createArrayBuffer(new Uint16Array(e.less_ls_index),L.gl.STATIC_DRAW)}},{key:"paint",value:function(){switch(L.gl.bindBuffer(L.gl.ARRAY_BUFFER,this.vertexBuffer),L.gl.vertexAttribPointer(F.vertex_position,3,L.gl.FLOAT,!1,24,0),L.gl.vertexAttribPointer(F.vertex_normal,3,L.gl.FLOAT,!1,24,0),L.gl.vertexAttribPointer(F.color,3,L.gl.FLOAT,!1,24,12),L.gl.uniformMatrix4fv(B.m_matrix,!1,this.mMatrix),L.gl.uniformMatrix4fv(B.pmv_matrix,!1,this.mvpMatrix),this.drawMode){case this.MODE_LINE:L.gl.uniform1f(B.alpha,this.line_alpha),L.gl.bindBuffer(L.gl.ELEMENT_ARRAY_BUFFER,this.lineIndexBuffer),L.gl.drawElements(L.gl.LINES,this.lineIndexBuffer.numItems,L.gl.UNSIGNED_SHORT,0);break;case this.MODE_LESSLINE:L.gl.uniform1f(B.alpha,this.line_alpha),L.gl.bindBuffer(L.gl.ELEMENT_ARRAY_BUFFER,this.lessLineIndexBuffer),L.gl.drawElements(L.gl.LINES,this.lessLineIndexBuffer.numItems,L.gl.UNSIGNED_SHORT,0),L.gl.bindBuffer(L.gl.ELEMENT_ARRAY_BUFFER,this.lessLSIndexBuffer),L.gl.drawElements(L.gl.TRIANGLES,this.lessLSIndexBuffer.numItems,L.gl.UNSIGNED_SHORT,0);break;case this.MODE_SURFACE:default:L.gl.uniform1f(B.alpha,this.alpha),L.gl.bindBuffer(L.gl.ELEMENT_ARRAY_BUFFER,this.indexBuffer),L.gl.drawElements(L.gl.TRIANGLES,this.indexBuffer.numItems,L.gl.UNSIGNED_SHORT,0);break}}},{key:"onViewChanged",value:function(t){this.pvMatrix=t||this.pvMatrix,mat4.mul(this.mvpMatrix,this.pvMatrix,this.mMatrix)}},{key:"setSize",value:function(t){this.size=t||this.DEFAULT_RADIUS,vec3.set(this.vscale,this.size,this.size,this.size),mat4.fromScaling(this.mMatrix,this.vscale),this.onViewChanged()}},{key:"setDrawMode",value:function(t){this.drawMode=t}},{key:"setAlpha",value:function(t){this.alpha=t}},{key:"getVertex",value:function(){var t,e,i,r=[],n=[];for(e=0;e<=this.vn;e++)for(t=0;t<=this.hn;t++)i=V(j(180*t/this.hn),j(360*e/this.vn),1),n=n.concat(i[0],i[1],i[2]);return n=n.concat([0,0,0]),r=z(n,this.color),{vertices:r}}},{key:"getIndex",value:function(){var t=[],e=[],i=[],r=[],n=0,a=0;for(a=0;a<this.vn;a++)for(n=0;n<this.hn+1;n++)e.push(n+a*(this.hn+1),n+1+a*(this.hn+1),n+a*(this.hn+1),n+(a+1)*(this.hn+1)),t.push(n+a*(this.hn+1),n+1+a*(this.hn+1),n+1+(a+1)*(this.hn+1)),t.push(n+a*(this.hn+1),n+1+(a+1)*(this.hn+1),n+(a+1)*(this.hn+1));for(n=0;n<this.hn+1;n++)a=0,i.push(n+a*(this.hn+1),n+1+a*(this.hn+1)),a=.25*this.vn,i.push(n+a*(this.hn+1),n+1+a*(this.hn+1)),a=.5*this.vn,i.push(n+a*(this.hn+1),n+1+a*(this.hn+1)),a=.75*this.vn,i.push(n+a*(this.hn+1),n+1+a*(this.hn+1));for(a=0;a<this.vn;a++)n=this.hn/2,i.push(n+a*(this.hn+1),n+(a+1)*(this.hn+1));for(a=.5*this.vn;a<.75*this.vn;a++)r.push((this.vn+1)*(this.hn+1)),r.push(.5*this.hn+a*(this.hn+1)),r.push(.5*this.hn+(a+1)*(this.hn+1)),r.push((this.vn+1)*(this.hn+1)),r.push(.5*this.hn+(a+1)*(this.hn+1)),r.push(.5*this.hn+a*(this.hn+1));return{vertex_index:t,line_index:e,less_line_index:i,less_ls_index:r}}}]),e}(I),Q=function(t){function e(t){var i;Object(T["a"])(this,e),i=Object(O["a"])(this,Object(k["a"])(e).call(this,t)),i.dis=t.dis||4,i.visible=!1;var r=[],n=[0,1,0],a=[0,0,1],s=0,h=0,o=0;for(r[o]=new K({pos:[0,0,i.dis],color:n,gl:L.gl}),o++,s=0;s<=2;s++)for(h=0;h<8;h++)r[o]=new K(1===s&&h%2===0?{pos:[45*(s+1),45*h,i.dis],color:n,gl:L.gl}:{pos:[45*(s+1),45*h,i.dis],color:a,gl:L.gl}),o++;return r[o]=new K({pos:[180,0,i.dis],color:n,gl:L.gl}),i.circles=r,i.setScale(t.size||1),i.init(),i}return Object(w["a"])(e,t),Object(P["a"])(e,[{key:"init",value:function(){var t=1,e=U(t,this.sides,0,2*Math.PI),i=z(e,this.color,e),r=0,n=[];for(r=1;r<this.sides+1;r++)n.push(r);this.vertexBuffer=this.createArrayBuffer(new Float32Array(i),L.gl.STATIC_DRAW),this.indexBuffer=this.createArrayBuffer(new Uint16Array(n),L.gl.STATIC_DRAW)}},{key:"paint",value:function(){if(this.visible){L.gl.uniform1f(B.alpha,this.alpha),L.gl.uniform1i(B.specColor,1),L.gl.bindBuffer(L.gl.ARRAY_BUFFER,this.vertexBuffer),L.gl.vertexAttribPointer(F.vertex_position,3,L.gl.FLOAT,!1,36,0),L.gl.vertexAttribPointer(F.vertex_normal,3,L.gl.FLOAT,!1,36,12),L.gl.vertexAttribPointer(F.color,3,L.gl.FLOAT,!1,36,24),L.gl.bindBuffer(L.gl.ELEMENT_ARRAY_BUFFER,this.indexBuffer);for(var t=0;t<26;t++)L.gl.uniform3fv(B.vertexColor,this.circles[t].color),L.gl.uniformMatrix4fv(B.pmv_matrix,!1,this.circles[t].mvpMatrix),L.gl.drawElements(this.circles[t].drawMode,this.sides,L.gl.UNSIGNED_SHORT,0);L.gl.uniform1i(B.specColor,0)}}},{key:"onViewChanged",value:function(t){for(var e=0;e<26;e++)this.circles[e].onViewChanged(t)}},{key:"setDis",value:function(t){this.dis=t;for(var e=0;e<26;e++)this.circles[e].setTranslation(this.dis+.1)}},{key:"setScale",value:function(t){for(var e=0;e<26;e++)this.circles[e].setScale(t)}},{key:"onSphericalChanged",value:function(t){for(var e=0;e<26;e++)this.circles[e].isCurrent(t)}}]),e}(I),K=function(t){function e(t){var i;return Object(T["a"])(this,e),i=Object(O["a"])(this,Object(k["a"])(e).call(this,t)),i.type="RCircle",void 0!==t.pos&&(i.setTranslation(t.pos[2],j(t.pos[0]),j(t.pos[1])),i.setQuat(t.pos[0],t.pos[1])),i.current=!1,i.drawMode=L.gl.LINES,i}return Object(w["a"])(e,t),Object(P["a"])(e,[{key:"onViewChanged",value:function(t){this.pvMatrix=t||this.pvMatrix,mat4.fromRotationTranslationScale(this.mMatrix,this.quat,this.translation,this.vscale),mat4.mul(this.mvpMatrix,this.pvMatrix,this.mMatrix)}},{key:"setTranslation",value:function(t,e,i){void 0!==e&&(this.theta=e),void 0!==i&&(this.phi=i),this.translation=V(this.theta,this.phi,t),this.onViewChanged()}},{key:"setQuat",value:function(t,e,i){quat.fromEuler(this.quat,0,t,e),this.onViewChanged()}},{key:"setScale",value:function(t){this.size=t,this.vscale=vec3.fromValues(t,t,t),this.onViewChanged()}},{key:"isCurrent",value:function(t){if(Math.abs(t.theta-this.theta)<.02*Math.PI&&Math.abs(t.phi-this.phi)<.02*Math.PI){this.current=!0;var e=1.15*t.size;vec3.set(this.vscale,e,e,e),this.originColor=new Float32Array(this.color),this.color=[1,0,0],this.drawMode=L.gl.LINE_LOOP}else this.current&&(this.current=!1,vec3.set(this.vscale,this.size,this.size,this.size),this.color=this.originColor,this.drawMode=L.gl.LINES);this.onViewChanged()}}]),e}(I),tt="precision mediump float;\r\n\r\nattribute vec3 aVertexPosition;\r\nattribute vec3 aVertexNormal;\r\nattribute vec2 aTexture;\r\nattribute vec3 aColor;\r\n\r\nuniform mat4 uPMVMatrix;\r\nuniform mat4 uMMatrix;\r\nuniform mat4 uNMatrix;\r\nuniform vec3 uVertColor;\r\nuniform bool uSpecColor;\r\n\r\nuniform vec3  uLightDirection; // 直射光的方向\r\nvarying vec3  vLight;\r\nvarying vec2  vTexture;\r\nvarying vec3  vNormal;\r\nvarying vec3  vPosition;\r\n\r\n\r\nvoid main(void) {\r\n    gl_Position = uPMVMatrix * vec4(aVertexPosition, 1.0);\r\n\r\n    vec3 normal = normalize( vec3( uNMatrix * vec4( aVertexNormal, 1 ) ) );\r\n    vNormal = normal;\r\n    vPosition = vec3( uMMatrix * vec4( aVertexPosition , 1.0 ) );\r\n\r\n//    vec3 ambientLight     = vec3( 0.2, 0.24, 0.21 );\r\n//    vec3 directLightColor = vec3( 0.75, 0.75, 0.75 );\r\n//    float directional = max( dot( normal, normalize( uLightDirection ) ), 0.0);\r\n//    vec3 diffuse = directLightColor * directional;\r\n\r\n    if( !uSpecColor ) {\r\n//        vLight = aColor * (ambientLight + diffuse);\r\n        vLight = aColor;\r\n    } else {\r\n//        vLight = uVertColor * (ambientLight + diffuse);\r\n        vLight = uVertColor;\r\n    }\r\n    vTexture = aTexture;\r\n}\r\n",et="precision mediump float;\r\n\r\nvarying vec3  vLight;\r\nvarying vec2  vTexture;\r\nvarying vec3  vNormal;\r\nvarying vec3  vPosition;\r\n\r\nuniform float uAlpha;\r\nuniform vec3 uFragColor;\r\nuniform bool uHasTexture;\r\nuniform sampler2D uSampler;\r\n\r\n// uniform sampler2D uYSampler;\r\n// uniform sampler2D uZSampler;\r\n// uniform int uEnableTexture;\r\nvoid main(void) {\r\n    vec3 lightPos = vec3( 10, 10, 10 );\r\n    vec3 ambientLight = vec3( 0.28, 0.28, 0.28 );\r\n    vec3 directLightColor = vec3( 0.88, 0.88, 0.88 );\r\n    float directional = max( dot( normalize( vNormal ), normalize( lightPos - vPosition ) ), 0.0);        // 直接使用顶点的法线数据进行漫反射计算\r\n    vec3 diffuse = directLightColor * directional;\r\n\r\n    if( uHasTexture ) {\r\n//        vec4 color = mix( uFragColor, texture2D( uSampler, vec2( vTexture.s, vTexture.t ) ), 0.5 );\r\n        gl_FragColor = vec4( uFragColor * (diffuse + ambientLight), uAlpha );\r\n    }\r\n    else {\r\n        gl_FragColor = vec4( vLight * (diffuse + ambientLight), uAlpha );\r\n    }\r\n//   mediump vec4 xtextureColor = texture2D(uXSampler, vec2(vXTexture.s, vXTexture.t));\r\n//   mediump vec4 ytextureColor = texture2D(uYSampler, vec2(vXTexture.s, vXTexture.t));\r\n//   mediump vec4 ztextureColor = texture2D(uZSampler, vec2(vXTexture.s, vXTexture.t));\r\n//   if( uEnableTexture == 0 ) {\r\n//     gl_FragColor = vec4(vLight, uAlpha);\r\n//   }\r\n//   else if( uEnableTexture == 1 ) {\r\n//     gl_FragColor = vec4(vLight, 1.0) * xtextureColor;\r\n//   }\r\n//   else if( uEnableTexture == 2 ) {\r\n//     gl_FragColor = vec4(vLight, 1.0) * ytextureColor;}\r\n//   else if( uEnableTexture == 3 ) {\r\n//     gl_FragColor = vec4(vLight, 1.0) * ztextureColor;\r\n//   }\r\n}\r\n",it=function(){function t(){Object(T["a"])(this,t)}return Object(P["a"])(t,[{key:"init",value:function(t,e){e||(e="webgl"),L.gl=t.getContext(e,{depth:!0,antilias:!0});var i=new H({});this.scene=i,i.initShaders(tt,et),this.camera=new G({height:600,width:800}),i.addCamera(this.camera),this.coord=new W({}),i.add(this.coord);var r=new q({color:[1,.2,.1],size:.4});r.setParam({dis:4,pitch:0,roll:0,heading:0}),this.sensorPoint=r;var n=new X({color:[.9,.5,.2]});this.sensorPath=n,this.sphere=new J({color:[.95,.2,.2],vn:48,hn:48});var a=new $({});this.recordPoint=a;var s=new Q({});this.refCircle=s,i.add(this.sensorPoint),i.add(this.sensorPath),i.add(this.recordPoint),i.add(this.refCircle),i.add(this.sphere),r.addParamCallback(function(t){n.onSphericalChanged(t),a.onSphericalChanged(t),s.onSphericalChanged(t)}),i.render()}},{key:"reset",value:function(){sensorPoint.reset(),sensorPath.resetAllPath(),camera.reset()}},{key:"addRefCircle",value:function(t){void 0===refCircle&&(refCircle=new Q(t),sensorPoint.addParamCallback(function(t){refCircle.onSphericalChanged(t)}),sensorPoint.update(),scene.add(refCircle,!0))}},{key:"addCraft",value:function(t){void 0===craft&&(craft=new Z(t),sensorPoint.addParamCallback(function(t){craft.setRotation(t)}),sensorPoint.update(),scene.add(craft,!0))}},{key:"paintGL",value:function(t){var e=t.devicePixelRatio,i=t.width*e,r=t.height*e;i===camera.width&&r===camera.height||camera.setSize(i,r),scene.render()}},{key:"resizeGL",value:function(t){var e=t.devicePixelRatio;t.pixelSize=Qt.size(t.width*e,t.height*e)}}]),t}();window.SpacePath=it;var rt={name:"app",components:{CSidebar:g,CInformation:A,CDebugPanel:C},data:function(){return{}},created:function(){},mounted:function(){console.log("Mounted"),this.spacepath=new it,this.spacepath.init(this.$refs.canvas),this.$refs.sidebar.init(this.spacepath)},methods:{debugValueChanged:function(t){"heading"==t.key?this.spacepath.sensorPoint.setParam({heading:h()(t.value)}):"pitch"==t.key?this.spacepath.sensorPoint.setParam({pitch:h()(t.value)}):"roll"==t.key&&this.spacepath.sensorPoint.setParam({roll:h()(t.value)})}}},nt=rt,at=(i("034f"),Object(v["a"])(nt,n,a,!1,null,null,null)),st=at.exports,ht=i("9f7b"),ot=i.n(ht);i("f9e3"),i("2dd8");r["a"].use(ot.a),r["a"].config.productionTip=!1,new r["a"]({render:function(t){return t(st)}}).$mount("#app")},"64a9":function(t,e,i){}});
//# sourceMappingURL=index.055ef945.js.map