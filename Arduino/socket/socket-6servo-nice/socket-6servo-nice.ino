#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
#include <Servo.h>

//#define DEBUG

//adjust this <<<<<<<<<<--------------
#define SERVO1   4
#define SERVO2   5
#define SERVO3   16
#define SERVO4   14
#define SERVO5   13
#define SERVO6   12

const bool invert_servo1 = true;
const bool invert_servo2 = false;
const bool invert_servo3 = false;
const bool invert_servo4 = false;
const bool invert_servo5 = false;
const bool invert_servo6 = false;

const char *ssid = "miharix_WiFi_RC";
const char *password = "mihamiha";

uint32_t failsaveS=5; //seconds whithout new input till failsave
const uint8_t failSaveServo1 = 40; //failsave position 0-180
const uint8_t failSaveServo2 = 90;
const uint8_t failSaveServo3 = 90;
const uint8_t failSaveServo4 = 90;
const uint8_t failSaveServo5 = 90;
const uint8_t failSaveServo6 = 90;

const uint8_t initSaveServo1 = 90; //startup servo position 0-180
const uint8_t initSaveServo2 = 90;
const uint8_t initSaveServo3 = 90;
const uint8_t initSaveServo4 = 90;
const uint8_t initSaveServo5 = 90;
const uint8_t initSaveServo6 = 90;
//end of adjusting, more is at the botom



#ifdef DEBUG
#define USE_SERIAL Serial
#endif
const long interval = 1000;
int same=0;


//nipples https://github.com/yoannmoinet/nipplejs

const char skripta[] PROGMEM = "<!DOCTYPE html>"
"<html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1, maximum-scale=1, user-scalable=0'/>"
"<title>Miharix WiFi RC</title>"
"<script>"    
    "var Servo1=0;"
    "var Servo2=0;"
    "var Servo3=0;"
    "var Servo4=0;"
    "var Servo5=0;"
    "var Servo6=0;"
    "var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);"
    "connection.onopen = function () {"
     "connection.send('Connect ' + new Date());"
    "};"
    "connection.onerror = function (error) {"
      "console.log('WebSocket Error ', error);"
    "};"
    "connection.onmessage = function (e) {"
      "console.log('Server: ', e.data);"
    "};"
    "function sendServo(){"
      "S1 =parseInt(Servo1).toString(16);"
      "S2 =parseInt(Servo2).toString(16);"
      "S3 =parseInt(Servo3).toString(16);"
      "S4 =parseInt(Servo4).toString(16);"
      "var S5 = parseInt(document.getElementById('S5').value).toString(16);"
      "var S6 = parseInt(document.getElementById('S6').value).toString(16);"
      "if(S1.length < 2) { S1 = '0' + S1; }"
      "if(S2.length < 2) { S2 = '0' + S2; }"
      "if(S3.length < 2) { S3 = '0' + S3; }"
      "if(S4.length < 2) { S4 = '0' + S4; }"
      "if(S5.length < 2) { S5 = '0' + S5; }"
      "if(S6.length < 2) { S6 = '0' + S6; }"
      "var S1_6 = S1+S2+S3+S4+S5+S6;"
      "console.log('S1_6: ' + S1_6);"
      "connection.send('S'+S1_6+'\\n');"
     "}"
     "</script>"
     
     
     "<script charset='utf-8'>!function(t){if(\"object\"==typeof exports&&\"undefined\"!=typeof module)module.exports=t();else if(\"function\"==typeof define&&define.amd)define([],t);else{var e;e=\"undefined\"!=typeof window?window:\"undefined\"!=typeof global?global:\"undefined\"!=typeof self?self:this,e.nipplejs=t()}}(function(){function t(){}function e(t,i){return this.identifier=i.identifier,this.position=i.position,this.frontPosition=i.frontPosition,this.collection=t,this.defaults={size:100,threshold:.1,color:\"white\",fadeTime:250,dataOnly:!1,restOpacity:.5,mode:\"dynamic\",zone:document.body},this.config(i),\"dynamic\"===this.options.mode&&(this.options.restOpacity=0),this.id=e.id,e.id+=1,this.buildEl().stylize(),this.instance={el:this.ui.el,on:this.on.bind(this),off:this.off.bind(this),show:this.show.bind(this),hide:this.hide.bind(this),add:this.addToDom.bind(this),remove:this.removeFromDom.bind(this),destroy:this.destroy.bind(this),resetDirection:this.resetDirection.bind(this),computeDirection:this.computeDirection.bind(this),trigger:this.trigger.bind(this),position:this.position,frontPosition:this.frontPosition,ui:this.ui,identifier:this.identifier,id:this.id,options:this.options},this.instance}function i(t,e){var n=this;return n.nipples=[],n.idles=[],n.actives=[],n.ids=[],n.pressureIntervals={},n.manager=t,n.id=i.id,i.id+=1,n.defaults={zone:document.body,multitouch:!1,maxNumberOfNipples:10,mode:\"dynamic\",position:{top:0,left:0},catchDistance:200,size:100,threshold:.1,color:\"white\",fadeTime:250,dataOnly:!1,restOpacity:.5},n.config(e),\"static\"!==n.options.mode&&\"semi\"!==n.options.mode||(n.options.multitouch=!1),n.options.multitouch||(n.options.maxNumberOfNipples=1),n.updateBox(),n.prepareNipples(),n.bindings(),n.begin(),n.nipples}function n(t){var e=this;e.ids={},e.index=0,e.collections=[],e.config(t),e.prepareCollections();var i;return c.bindEvt(window,\"resize\",function(t){clearTimeout(i),i=setTimeout(function(){var t,i=c.getScroll();e.collections.forEach(function(e){e.forEach(function(e){t=e.el.getBoundingClientRect(),e.position={x:i.x+t.left,y:i.y+t.top}})})},100)}),e.collections}var o,r=!!(\"ontouchstart\"in window),s=!!window.PointerEvent,d=!!window.MSPointerEvent,a={touch:{start:\"touchstart\",move:\"touchmove\",end:\"touchend\"},mouse:{start:\"mousedown\",move:\"mousemove\",end:\"mouseup\"},pointer:{start:\"pointerdown\",move:\"pointermove\",end:\"pointerup\"},MSPointer:{start:\"MSPointerDown\",move:\"MSPointerMove\",end:\"MSPointerUp\"}},p={};s?o=a.pointer:d?o=a.MSPointer:r?(o=a.touch,p=a.mouse):o=a.mouse;var c={};c.distance=function(t,e){var i=e.x-t.x,n=e.y-t.y;return Math.sqrt(i*i+n*n)},c.angle=function(t,e){var i=e.x-t.x,n=e.y-t.y;return c.degrees(Math.atan2(n,i))},c.findCoord=function(t,e,i){var n={x:0,y:0};return i=c.radians(i),n.x=t.x-e*Math.cos(i),n.y=t.y-e*Math.sin(i),n},c.radians=function(t){return t*(Math.PI/180)},c.degrees=function(t){return t*(180/Math.PI)},c.bindEvt=function(t,e,i){t.addEventListener?t.addEventListener(e,i,!1):t.attachEvent&&t.attachEvent(e,i)},c.unbindEvt=function(t,e,i){t.removeEventListener?t.removeEventListener(e,i):t.detachEvent&&t.detachEvent(e,i)},c.trigger=function(t,e,i){var n=new CustomEvent(e,i);t.dispatchEvent(n)},c.prepareEvent=function(t){return t.preventDefault(),t.type.match(/^touch/)?t.changedTouches:t},c.getScroll=function(){var t=void 0!==window.pageXOffset?window.pageXOffset:(document.documentElement||document.body.parentNode||document.body).scrollLeft,e=void 0!==window.pageYOffset?window.pageYOffset:(document.documentElement||document.body.parentNode||document.body).scrollTop;return{x:t,y:e}},c.applyPosition=function(t,e){e.x&&e.y?(t.style.left=e.x+\"px\",t.style.top=e.y+\"px\"):(e.top||e.right||e.bottom||e.left)&&(t.style.top=e.top,t.style.right=e.right,t.style.bottom=e.bottom,t.style.left=e.left)},c.getTransitionStyle=function(t,e,i){var n=c.configStylePropertyObject(t);for(var o in n)if(n.hasOwnProperty(o))if(\"string\"==typeof e)n[o]=e+\" \"+i;else{for(var r=\"\",s=0,d=e.length;s<d;s+=1)r+=e[s]+\" \"+i+\", \";n[o]=r.slice(0,-2)}return n},c.getVendorStyle=function(t,e){var i=c.configStylePropertyObject(t);for(var n in i)i.hasOwnProperty(n)&&(i[n]=e);return i},c.configStylePropertyObject=function(t){var e={};e[t]=\"\";var i=[\"webkit\",\"Moz\",\"o\"];return i.forEach(function(i){e[i+t.charAt(0).toUpperCase()+t.slice(1)]=\"\"}),e},c.extend=function(t,e){for(var i in e)e.hasOwnProperty(i)&&(t[i]=e[i]);return t},c.safeExtend=function(t,e){var i={};for(var n in t)t.hasOwnProperty(n)&&e.hasOwnProperty(n)?i[n]=e[n]:t.hasOwnProperty(n)&&(i[n]=t[n]);return i},c.map=function(t,e){if(t.length)for(var i=0,n=t.length;i<n;i+=1)e(t[i]);else e(t)},t.prototype.on=function(t,e){var i,n=this,o=t.split(/[ ,]+/g);n._handlers_=n._handlers_||{};for(var r=0;r<o.length;r+=1)i=o[r],n._handlers_[i]=n._handlers_[i]||[],n._handlers_[i].push(e);return n},t.prototype.off=function(t,e){var i=this;return i._handlers_=i._handlers_||{},void 0===t?i._handlers_={}:void 0===e?i._handlers_[t]=null:i._handlers_[t]&&i._handlers_[t].indexOf(e)>=0&&i._handlers_[t].splice(i._handlers_[t].indexOf(e),1),i},t.prototype.trigger=function(t,e){var i,n=this,o=t.split(/[ ,]+/g);n._handlers_=n._handlers_||{};for(var r=0;r<o.length;r+=1)i=o[r],n._handlers_[i]&&n._handlers_[i].length&&n._handlers_[i].forEach(function(t){t.call(n,{type:i,target:n},e)})},t.prototype.config=function(t){var e=this;e.options=e.defaults||{},t&&(e.options=c.safeExtend(e.options,t))},t.prototype.bindEvt=function(t,e){var i=this;return i._domHandlers_=i._domHandlers_||{},i._domHandlers_[e]=function(){\"function\"==typeof i[\"on\"+e]?i[\"on\"+e].apply(i,arguments):console.warn('[WARNING] : Missing \"on'+e+'\" handler.')},c.bindEvt(t,o[e],i._domHandlers_[e]),p[e]&&c.bindEvt(t,p[e],i._domHandlers_[e]),i},t.prototype.unbindEvt=function(t,e){var i=this;return i._domHandlers_=i._domHandlers_||{},c.unbindEvt(t,o[e],i._domHandlers_[e]),p[e]&&c.unbindEvt(t,p[e],i._domHandlers_[e]),delete i._domHandlers_[e],this},e.prototype=new t,e.constructor=e,e.id=0,e.prototype.buildEl=function(t){return this.ui={},this.options.dataOnly?this:(this.ui.el=document.createElement(\"div\"),this.ui.back=document.createElement(\"div\"),this.ui.front=document.createElement(\"div\"),this.ui.el.className=\"nipple collection_\"+this.collection.id,this.ui.back.className=\"back\",this.ui.front.className=\"front\",this.ui.el.setAttribute(\"id\",\"nipple_\"+this.collection.id+\"_\"+this.id),this.ui.el.appendChild(this.ui.back),this.ui.el.appendChild(this.ui.front),this)},e.prototype.stylize=function(){if(this.options.dataOnly)return this;var t=this.options.fadeTime+\"ms\",e=c.getVendorStyle(\"borderRadius\",\"50%\"),i=c.getTransitionStyle(\"transition\",\"opacity\",t),n={};return n.el={position:\"absolute\",opacity:this.options.restOpacity,display:\"block\",zIndex:999},n.back={position:\"absolute\",display:\"block\",width:this.options.size+\"px\",height:this.options.size+\"px\",marginLeft:-this.options.size/2+\"px\",marginTop:-this.options.size/2+\"px\",background:this.options.color,opacity:\".5\"},n.front={width:this.options.size/2+\"px\",height:this.options.size/2+\"px\",position:\"absolute\",display:\"block\",marginLeft:-this.options.size/4+\"px\",marginTop:-this.options.size/4+\"px\",background:this.options.color,opacity:\".5\"},c.extend(n.el,i),c.extend(n.back,e),c.extend(n.front,e),this.applyStyles(n),this},e.prototype.applyStyles=function(t){for(var e in this.ui)if(this.ui.hasOwnProperty(e))for(var i in t[e])this.ui[e].style[i]=t[e][i];return this},e.prototype.addToDom=function(){return this.options.dataOnly||document.body.contains(this.ui.el)?this:(this.options.zone.appendChild(this.ui.el),this)},e.prototype.removeFromDom=function(){return this.options.dataOnly||!document.body.contains(this.ui.el)?this:(this.options.zone.removeChild(this.ui.el),this)},e.prototype.destroy=function(){clearTimeout(this.removeTimeout),clearTimeout(this.showTimeout),clearTimeout(this.restTimeout),this.trigger(\"destroyed\",this.instance),this.removeFromDom(),this.off()},e.prototype.show=function(t){var e=this;return e.options.dataOnly?e:(clearTimeout(e.removeTimeout),clearTimeout(e.showTimeout),clearTimeout(e.restTimeout),e.addToDom(),e.restCallback(),setTimeout(function(){e.ui.el.style.opacity=1},0),e.showTimeout=setTimeout(function(){e.trigger(\"shown\",e.instance),\"function\"==typeof t&&t.call(this)},e.options.fadeTime),e)},e.prototype.hide=function(t){var e=this;return e.options.dataOnly?e:(e.ui.el.style.opacity=e.options.restOpacity,clearTimeout(e.removeTimeout),clearTimeout(e.showTimeout),clearTimeout(e.restTimeout),e.removeTimeout=setTimeout(function(){var i=\"dynamic\"===e.options.mode?\"none\":\"block\";e.ui.el.style.display=i,\"function\"==typeof t&&t.call(e),e.trigger(\"hidden\",e.instance)},e.options.fadeTime),e.restPosition(),e)},e.prototype.restPosition=function(t){var e=this;e.frontPosition={x:0,y:0};var i=e.options.fadeTime+\"ms\",n={};n.front=c.getTransitionStyle(\"transition\",[\"top\",\"left\"],i);var o={front:{}};o.front={left:e.frontPosition.x+\"px\",top:e.frontPosition.y+\"px\"},e.applyStyles(n),e.applyStyles(o),e.restTimeout=setTimeout(function(){\"function\"==typeof t&&t.call(e),e.restCallback()},e.options.fadeTime)},e.prototype.restCallback=function(){var t=this,e={};e.front=c.getTransitionStyle(\"transition\",\"none\",\"\"),t.applyStyles(e),t.trigger(\"rested\",t.instance)},e.prototype.resetDirection=function(){this.direction={x:!1,y:!1,angle:!1}},e.prototype.computeDirection=function(t){var e,i,n,o=t.angle.radian,r=Math.PI/4,s=Math.PI/2;if(e=o>r&&o<3*r?\"up\":o>-r&&o<=r?\"left\":o>3*-r&&o<=-r?\"down\":\"right\",i=o>-s&&o<s?\"left\":\"right\",n=o>0?\"up\":\"down\",t.force>this.options.threshold){var d={};for(var a in this.direction)this.direction.hasOwnProperty(a)&&(d[a]=this.direction[a]);var p={};this.direction={x:i,y:n,angle:e},t.direction=this.direction;for(var a in d)d[a]===this.direction[a]&&(p[a]=!0);if(p.x&&p.y&&p.angle)return t;p.x&&p.y||this.trigger(\"plain\",t),p.x||this.trigger(\"plain:\"+i,t),p.y||this.trigger(\"plain:\"+n,t),p.angle||this.trigger(\"dir dir:\"+e,t)}return t},i.prototype=new t,i.constructor=i,i.id=0,i.prototype.prepareNipples=function(){var t=this,e=t.nipples;e.on=t.on.bind(t),e.off=t.off.bind(t),e.options=t.options,e.destroy=t.destroy.bind(t),e.ids=t.ids,e.id=t.id,e.processOnMove=t.processOnMove.bind(t),e.processOnEnd=t.processOnEnd.bind(t),e.get=function(t){if(void 0===t)return e[0];for(var i=0,n=e.length;i<n;i+=1)if(e[i].identifier===t)return e[i];return!1}},i.prototype.bindings=function(){var t=this;t.bindEvt(t.options.zone,\"start\"),t.options.zone.style.touchAction=\"none\",t.options.zone.style.msTouchAction=\"none\"},i.prototype.begin=function(){var t=this,e=t.options;if(\"static\"===e.mode){var i=t.createNipple(e.position,t.manager.getIdentifier());i.add(),t.idles.push(i)}},i.prototype.createNipple=function(t,i){var n=this,o=c.getScroll(),r={},s=n.options;if(t.x&&t.y)r={x:t.x-(o.x+n.box.left),y:t.y-(o.y+n.box.top)};else if(t.top||t.right||t.bottom||t.left){var d=document.createElement(\"DIV\");d.style.display=\"hidden\",d.style.top=t.top,d.style.right=t.right,d.style.bottom=t.bottom,d.style.left=t.left,d.style.position=\"absolute\",s.zone.appendChild(d);var a=d.getBoundingClientRect();s.zone.removeChild(d),r=t,t={x:a.left+o.x,y:a.top+o.y}}var p=new e(n,{color:s.color,size:s.size,threshold:s.threshold,fadeTime:s.fadeTime,dataOnly:s.dataOnly,restOpacity:s.restOpacity,mode:s.mode,identifier:i,position:t,zone:s.zone,frontPosition:{x:0,y:0}});return s.dataOnly||(c.applyPosition(p.ui.el,r),c.applyPosition(p.ui.front,p.frontPosition)),n.nipples.push(p),n.trigger(\"added \"+p.identifier+\":added\",p),n.manager.trigger(\"added \"+p.identifier+\":added\",p),n.bindNipple(p),p},i.prototype.updateBox=function(){var t=this;t.box=t.options.zone.getBoundingClientRect()},i.prototype.bindNipple=function(t){var e,i=this,n=function(t,n){e=t.type+\" \"+n.id+\":\"+t.type,i.trigger(e,n)};t.on(\"destroyed\",i.onDestroyed.bind(i)),t.on(\"shown hidden rested dir plain\",n),t.on(\"dir:up dir:right dir:down dir:left\",n),t.on(\"plain:up plain:right plain:down plain:left\",n)},i.prototype.pressureFn=function(t,e,i){var n=this,o=0;clearInterval(n.pressureIntervals[i]),n.pressureIntervals[i]=setInterval(function(){var i=t.force||t.pressure||t.webkitForce||0;i!==o&&(e.trigger(\"pressure\",i),n.trigger(\"pressure \"+e.identifier+\":pressure\",i),o=i)}.bind(n),100)},i.prototype.onstart=function(t){var e=this,i=e.options;t=c.prepareEvent(t),e.updateBox();var n=function(t){e.actives.length<i.maxNumberOfNipples&&e.processOnStart(t)};return c.map(t,n),e.manager.bindDocument(),!1},i.prototype.processOnStart=function(t){var e,i=this,n=i.options,o=i.manager.getIdentifier(t),r=t.force||t.pressure||t.webkitForce||0,s={x:t.pageX,y:t.pageY},d=i.getOrCreate(o,s);d.identifier=o;var a=function(e){e.trigger(\"start\",e),i.trigger(\"start \"+e.id+\":start\",e),e.show(),r>0&&i.pressureFn(t,e,e.identifier),i.processOnMove(t)};if((e=i.idles.indexOf(d))>=0&&i.idles.splice(e,1),i.actives.push(d),i.ids.push(d.identifier),\"semi\"!==n.mode)a(d);else{var p=c.distance(s,d.position);if(!(p<=n.catchDistance))return d.destroy(),void i.processOnStart(t);a(d)}return d},i.prototype.getOrCreate=function(t,e){var i,n=this,o=n.options;return/(semi|static)/.test(o.mode)?(i=n.idles[0])?(n.idles.splice(0,1),i):\"semi\"===o.mode?n.createNipple(e,t):(console.warn(\"Coudln't find the needed nipple.\"),!1):i=n.createNipple(e,t)},i.prototype.processOnMove=function(t){var e=this,i=e.options,n=e.manager.getIdentifier(t),o=e.nipples.get(n);if(!o)return console.error(\"Found zombie joystick with ID \"+n),void e.manager.removeIdentifier(n);o.identifier=n;var r=o.options.size/2,s={x:t.pageX,y:t.pageY},d=c.distance(s,o.position),a=c.angle(s,o.position),p=c.radians(a),l=d/r;d>r&&(d=r,s=c.findCoord(o.position,d,a)),o.frontPosition={x:s.x-o.position.x,y:s.y-o.position.y},i.dataOnly||c.applyPosition(o.ui.front,o.frontPosition);var u={identifier:o.identifier,position:s,force:l,pressure:t.force||t.pressure||t.webkitForce||0,distance:d,angle:{radian:p,degree:a},instance:o};u=o.computeDirection(u),u.angle={radian:c.radians(180-a),degree:180-a},o.trigger(\"move\",u),e.trigger(\"move \"+o.id+\":move\",u)},i.prototype.processOnEnd=function(t){var e=this,i=e.options,n=e.manager.getIdentifier(t),o=e.nipples.get(n),r=e.manager.removeIdentifier(o.identifier);o&&(i.dataOnly||o.hide(function(){\"dynamic\"===i.mode&&(o.trigger(\"removed\",o),e.trigger(\"removed \"+o.id+\":removed\",o),e.manager.trigger(\"removed \"+o.id+\":removed\",o),o.destroy())}),clearInterval(e.pressureIntervals[o.identifier]),o.resetDirection(),o.trigger(\"end\",o),e.trigger(\"end \"+o.id+\":end\",o),e.ids.indexOf(o.identifier)>=0&&e.ids.splice(e.ids.indexOf(o.identifier),1),e.actives.indexOf(o)>=0&&e.actives.splice(e.actives.indexOf(o),1),/(semi|static)/.test(i.mode)?e.idles.push(o):e.nipples.indexOf(o)>=0&&e.nipples.splice(e.nipples.indexOf(o),1),e.manager.unbindDocument(),/(semi|static)/.test(i.mode)&&(e.manager.ids[r.id]=r.identifier))},i.prototype.onDestroyed=function(t,e){var i=this;i.nipples.indexOf(e)>=0&&i.nipples.splice(i.nipples.indexOf(e),1),i.actives.indexOf(e)>=0&&i.actives.splice(i.actives.indexOf(e),1),i.idles.indexOf(e)>=0&&i.idles.splice(i.idles.indexOf(e),1),i.ids.indexOf(e.identifier)>=0&&i.ids.splice(i.ids.indexOf(e.identifier),1),i.manager.removeIdentifier(e.identifier),i.manager.unbindDocument()},i.prototype.destroy=function(){var t=this;t.unbindEvt(t.options.zone,\"start\"),t.nipples.forEach(function(t){t.destroy()});for(var e in t.pressureIntervals)t.pressureIntervals.hasOwnProperty(e)&&clearInterval(t.pressureIntervals[e]);t.trigger(\"destroyed\",t.nipples),t.manager.unbindDocument(),t.off()},n.prototype=new t,n.constructor=n,n.prototype.prepareCollections=function(){var t=this;t.collections.create=t.create.bind(t),t.collections.on=t.on.bind(t),t.collections.off=t.off.bind(t),t.collections.destroy=t.destroy.bind(t),t.collections.get=function(e){var i;return t.collections.every(function(t){return!(i=t.get(e))}),i}},n.prototype.create=function(t){return this.createCollection(t)},n.prototype.createCollection=function(t){var e=this,n=new i(e,t);return e.bindCollection(n),e.collections.push(n),n},n.prototype.bindCollection=function(t){var e,i=this,n=function(t,n){e=t.type+\" \"+n.id+\":\"+t.type,i.trigger(e,n)};t.on(\"destroyed\",i.onDestroyed.bind(i)),t.on(\"shown hidden rested dir plain\",n),t.on(\"dir:up dir:right dir:down dir:left\",n),t.on(\"plain:up plain:right plain:down plain:left\",n)},n.prototype.bindDocument=function(){var t=this;t.binded||(t.bindEvt(document,\"move\").bindEvt(document,\"end\"),t.binded=!0)},n.prototype.unbindDocument=function(t){var e=this;Object.keys(e.ids).length&&t!==!0||(e.unbindEvt(document,\"move\").unbindEvt(document,\"end\"),e.binded=!1)},n.prototype.getIdentifier=function(t){var e;return t?(e=void 0===t.identifier?t.pointerId:t.identifier,void 0===e&&(e=this.latest||0)):e=this.index,void 0===this.ids[e]&&(this.ids[e]=this.index,this.index+=1),this.latest=e,this.ids[e]},n.prototype.removeIdentifier=function(t){var e={};for(var i in this.ids)if(this.ids[i]===t){e.id=i,e.identifier=this.ids[i],delete this.ids[i];break}return e},n.prototype.onmove=function(t){var e=this;return e.onAny(\"move\",t),!1},n.prototype.onend=function(t){var e=this;return e.onAny(\"end\",t),!1},n.prototype.onAny=function(t,e){var i,n=this,o=\"processOn\"+t.charAt(0).toUpperCase()+t.slice(1);e=c.prepareEvent(e);var r=function(t,e,i){i.ids.indexOf(e)>=0&&(i[o](t),t._found_=!0)},s=function(t){i=n.getIdentifier(t),c.map(n.collections,r.bind(null,t,i)),t._found_||n.removeIdentifier(i)};return c.map(e,s),!1},n.prototype.destroy=function(){var t=this;t.unbindDocument(!0),t.ids={},t.index=0,t.collections.forEach(function(t){t.destroy()}),t.off()},n.prototype.onDestroyed=function(t,e){var i=this;return!(i.collections.indexOf(e)<0)&&void i.collections.splice(i.collections.indexOf(e),1)};var l=new n;return{create:function(t){return l.create(t)},factory:l}});</script>"


     
     "<style>"
     "html, body {"
            "position: absolute;"
            "top: 0;"
            "left: 0;"
            "right: 0;"
            "bottom: 0;"
            "padding: 0;"
            "margin: 0;"
            "overflow:hidden;"
        "}"
        "#leftB {"
            "position: absolute;"
            "left: 0;"
            "top: 0;"
            "height: 80%;"
            "width: 50%;"
            "background: black;"
        "}"
         "#leftT {"
            "position: absolute;"
            "left: 0;"
            "top: 80%;"
            "height: 20%;"
            "width: 50%;"
            "background: black;"
        "}"
        "#rightB {"
            "position: absolute;"
            "right: 0;"
            "top: 0;"
            "height: 80%;"
            "width: 50%;"
            "background: black;"
        "}"
        "#rightT {"
            "position: absolute;"
            "right: 0;"
            "top: 80%;"
            "height: 20%;"
            "width: 50%;"
            "background: black;"
        "}"
       ".button {"
     "display: inline-block;"
      "padding: 0px 0px;"
      "font-size: 100%;"
      "cursor: pointer;"
      "text-align: center;"
      "text-decoration: none;"
      "outline: 0 !important;"
      "color: #FF4000;"
      "background-color: #004000;"
      "border: none;"
      "border-radius:30px;"
      "position: absolute;"
      "width: 80%;"
      "height: 80%;"
      "top: 10%;"
      "right: 10%;"
  "}"
  "#RCName {"
      "position: absolute;"
      "pointer-events: none;"
            "right: 0;"
            "top: 0;"
            "height: 16px;"
            "width: 100%;"
            "background: black;"
            "z-index: 1;"
            "font-size: 15px;"
            "text-align: center;"
            "color: #555;"
  "}"
  "button::-moz-focus-inner{"
      "border: 0;"
  "}"
  ".button:active {"
      "background-color: #005000;"
      "color: #005000;"
      "transform: translateY(4px);"
  "}"
    "</style>"
    "</head><body>"
  "<div id='RCName'>Miharix WiFi RC</div>"
  "<div id='leftT'>"
   "<button id='S5' type='button' class='button' value=0 onclick='ButtonS5();'></button>"
  "</div>"
  "<div id='rightT'>"
    "<button id='S6' type='button' class='button' value=0 onclick='ButtonS6();'></button>"
  "</div>"
  "<div id='leftB'></div>"
  "<div id='rightB'></div>"

"<script>"
    "var w = window,"
    "d = document,"
    "e = d.documentElement,"
    "g = d.getElementsByTagName('body')[0],"
    "xs = w.innerWidth || e.clientWidth || g.clientWidth,"
    "ys = w.innerHeight|| e.clientHeight|| g.clientHeight;"
    "if(xs<ys){xs=ys;};"
    "nip_size=xs/4;"
    
    "max_rangeX1=180;"
    "min_rangeX1=0;"
    
    "max_rangeY1=180;"
    "min_rangeY1=0;"
    
    "max_rangeX2=180;"
    "min_rangeX2=0;"
    
    "max_rangeY2=180;"
    "min_rangeY2=0;"

      "var joystickL = nipplejs.create({"
  "zone: document.getElementById('leftB'),"
        "mode: 'static',"
        "position: { left: '50%', top: '50%' },"
        "color: 'green',"
        "size: nip_size,"
        "restOpacity: 1"
      "});"
      
      "var joystickR = nipplejs.create({"
  "zone: document.getElementById('rightB'),"
        "mode: 'static',"
        "position: { left: '50%', top: '50%' },"
        "color: 'green',"
        "size: nip_size,"
        "restOpacity: 1"
      "});"

      "joystickL.on('move',function (evt, data){"
  "if(((data.angle.degree<90)&&(data.angle.degree>0))||((data.angle.degree<360)&&(data.angle.degree>270))){"
    "X=Math.cos(data.angle.radian)*data.distance;"
    "Y=Math.sin(data.angle.radian)*data.distance;"
  "}"
  "if(((data.angle.degree<180)&&(data.angle.degree>90))||((data.angle.degree<270)&&(data.angle.degree>180))){"
    "X=-Math.cos(Math.PI-data.angle.radian)*data.distance;"
    "Y=Math.sin(Math.PI-data.angle.radian)*data.distance;"
  "}"
  "Servo1=((X+nip_size/2)*(max_rangeX1-min_rangeX1)/nip_size+min_rangeX1).toFixed(0);"
  "Servo2=((Y+nip_size/2)*(max_rangeY1-min_rangeY1)/nip_size+min_rangeY1).toFixed(0);"
  
  "sendServo();"
  //document.getElementById('RCName').innerHTML = Servo1+'<br>'+Servo2+'<br> X:'+ X.toFixed(0)+'<br> Y:'+ Y.toFixed(0)+'<br>a:'+data.angle.radian.toFixed(0)+'<br>D:'+data.distance.toFixed(0)+'<br>cos:'+Math.cos(data.angle.radian);
      "});"
        
      "joystickL.on('end',function (evt, data){"
  "Servo1=0;"
  "Servo2=0;"
  "sendServo();"
  //document.getElementById('RCName').innerHTML = ' X: 0, Y:0 '+x/8;
      "});"
      
      "joystickR.on('move',function (evt, data){"
  "if(((data.angle.degree<90)&&(data.angle.degree>0))||((data.angle.degree<360)&&(data.angle.degree>270))){"
    "X=Math.cos(data.angle.radian)*data.distance;"
    "Y=Math.sin(data.angle.radian)*data.distance;"
  "}"
  "if(((data.angle.degree<180)&&(data.angle.degree>90))||((data.angle.degree<270)&&(data.angle.degree>180))){"
    "X=-Math.cos(Math.PI-data.angle.radian)*data.distance;"
    "Y=Math.sin(Math.PI-data.angle.radian)*data.distance;"
  "}"
  "Servo3=((X+nip_size/2)*(max_rangeX2-min_rangeX2)/nip_size+min_rangeX2).toFixed(0);"
  "Servo4=((Y+nip_size/2)*(max_rangeY2-min_rangeY2)/nip_size+min_rangeY2).toFixed(0);"
  
  "sendServo();"
  //document.getElementById('RCName').innerHTML = Servo3+'<br>'+Servo4+'<br> X:'+ X.toFixed(0)+'<br> Y:'+ Y.toFixed(0)+'<br>a:'+data.angle.radian.toFixed(0)+'<br>D:'+data.distance.toFixed(0)+'<br>cos:'+Math.cos(data.angle.radian);
      "});"
        
      "joystickR.on('end',function (evt, data){"
  "Servo3=0;"
  "Servo4=0;"
  "sendServo();"
  //document.getElementById('RCName').innerHTML = ' X: 0, Y:0 '+x/8;
      "});"
      
      "function ButtonS5(){"
  "if(S5.value==0){"
    "S5.value=180;"
  //  S5.innerHTML='ON';
    "S5.style.backgroundColor='white';"
  "}else{"
    "S5.value=0;"
   // S5.innerHTML='OFF';
    "S5.style.backgroundColor='#005000';"
  "};}"
      
      "function ButtonS6(){"
  "if(S6.value==0){"
    "S6.value=180;"
   // S6.innerHTML='ON';
    "S6.style.backgroundColor='red';"
  "}else{"
    "S6.value=0;"
   // S6.innerHTML='OFF';
  "S6.style.backgroundColor='#005000';"
  "};}</script>"
  "<!--https://github.com/miharix/miharix-wifi-rc/-->"
  "<!--nipples https://github.com/yoannmoinet/nipplejs-->"
  "</body></html>"

  
;



Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

unsigned long previousMillis = 0;
//uint32_t S1_6_p = 0;
bool S1_6 = false;
char S1_buff[3];
char S2_buff[3];
char S3_buff[3];
char S4_buff[3];
char S5_buff[3];
char S6_buff[3];

ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:

            #ifdef DEBUG
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
            #endif

            digitalWrite(2, LOW);
            
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);

            #ifdef DEBUG
            USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            #endif
            
            // send message to client
            webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:

            #ifdef DEBUG
            USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);
            #endif
            
            if(payload[0] == 'S') {
                
                memcpy(S1_buff, &payload[1], 2 );
                S1_buff[3] = '\0';

                memcpy(S2_buff, &payload[3], 2 );
                S1_buff[3] = '\0';

                memcpy(S3_buff, &payload[5], 2 );
                S1_buff[3] = '\0';

                memcpy(S4_buff, &payload[7], 2 );
                S1_buff[3] = '\0';

                memcpy(S5_buff, &payload[9], 2 );
                S1_buff[3] = '\0';

                memcpy(S6_buff, &payload[11], 2 );
                S1_buff[3] = '\0';
  
                S1_6=true;
                 
                 #ifdef DEBUG
                  USE_SERIAL.printf(" S1_6=%d",S1_6);
                //  USE_SERIAL.printf(" S1_6_p=%u",S1_6_p);
                  USE_SERIAL.printf("\nS1_buff=%u",strtol(S1_buff,NULL,16));
                  USE_SERIAL.printf("\nS2_buff=%u",strtol(S2_buff,NULL,16));
                  USE_SERIAL.printf("\nS3_buff=%u",strtol(S3_buff,NULL,16));
                  USE_SERIAL.printf("\nS4_buff=%u",strtol(S4_buff,NULL,16));
                  USE_SERIAL.printf("\nS5_buff=%u",strtol(S5_buff,NULL,16));
                  USE_SERIAL.printf("\nS6_buff=%u",strtol(S6_buff,NULL,16));
                 #endif
              
               
               if(invert_servo1){
                servo1.write(180-(strtol(S1_buff,NULL,16)));
               }else{
                 servo1.write(strtol(S1_buff,NULL,16));
               }

               
               if(invert_servo2){
                  servo2.write(180-(strtol(S2_buff,NULL,16)));
               }else{
                 servo2.write(strtol(S2_buff,NULL,16));
               }

               if(invert_servo3){
                  servo3.write(180-(strtol(S3_buff,NULL,16)));
               }else{
                 servo3.write(strtol(S3_buff,NULL,16));
               }

               if(invert_servo4){
                  servo4.write(180-(strtol(S4_buff,NULL,16)));
               }else{
                 servo4.write(strtol(S4_buff,NULL,16));
               }

               if(invert_servo5){
                  servo5.write(180-(strtol(S5_buff,NULL,16)));
               }else{
                 servo5.write(strtol(S5_buff,NULL,16));
               }

               if(invert_servo6){
                  servo6.write(180-(strtol(S6_buff,NULL,16)));
               }else{
                 servo6.write(strtol(S6_buff,NULL,16));
               }

               
            }

            break;
    }

}

void setup() {
    
    #ifdef DEBUG
    USE_SERIAL.begin(115200);

    //USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
    #endif



    WiFi.softAP(ssid, password);

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    if(MDNS.begin("esp8266")) {
        #ifdef DEBUG
        USE_SERIAL.println("MDNS responder started");
        #endif
    }
    

    // handle index
    server.on("/", []() {
    server.send(200, "text/html", skripta);
    digitalWrite(2, LOW);
    delay(50); 
    digitalWrite(2, HIGH);  
    });

    server.begin();

    // Add service to MDNS
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);

//set servo init value <<<<<<<<<<<<<<<<--------------------------------------
  servo1.attach(SERVO1);
  servo1.write(initSaveServo1);
  servo2.attach(SERVO2);
  servo2.write(initSaveServo2);
  servo3.attach(SERVO3);
  servo3.write(initSaveServo3);
  servo4.attach(SERVO4);
  servo4.write(initSaveServo4);
  servo5.attach(SERVO5);
  servo5.write(initSaveServo5);
  servo6.attach(SERVO6);
  servo6.write(initSaveServo6);
//
  pinMode(2, OUTPUT);

  for(uint8_t t = 4; t > 0; t--) {
    digitalWrite(2, HIGH);
    delay(50); 
    digitalWrite(2, LOW);
    delay(50); 
  }

}

void loop() {
    webSocket.loop();
    
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
       #ifdef DEBUG
      USE_SERIAL.printf("S1_6=%d\n",S1_6);
      #endif
      previousMillis = currentMillis;
      if (S1_6 == false) {
        same++;
       if(same>=failsaveS){
          servo1.write(failSaveServo1);
          servo2.write(failSaveServo2);
          servo3.write(failSaveServo3);
          servo4.write(failSaveServo4);
          servo5.write(failSaveServo5);
          servo6.write(failSaveServo6);
          digitalWrite(2, LOW);
       }
      }else{
       digitalWrite(2, HIGH);
       same=0;
       S1_6 = false;
      }
    }

    
    server.handleClient();
}

