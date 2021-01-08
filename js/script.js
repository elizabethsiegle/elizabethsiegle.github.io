(function() {
  setExEventListener();
})();

function setExEventListener() {
  Array.from(document.getElementsByTagName("button")).forEach(e => {
    e.addEventListener("click", exToggle);
  });
}

function exToggle(e) {
  let exType = e.target;
  off(exType);
  let exTypeEl = document.getElementsByClassName(exType.id)[0];
  if (exTypeEl !== undefined) exTypeEl.classList.add("show");
}

function off(exType) {
  Array.from(document.getElementsByTagName("button")).forEach(butt => {
    butt.style.borderColor = "#96979c";
    butt.style.color = "#96979c";
  });
  Array.from(document.getElementsByClassName("ex")).forEach(e => {
    e.classList.remove("show");
  });
}


// Select the button
const btn = document.querySelector('.btn-toggle');

// Listen for a click on the button
btn.addEventListener('click', function() {
  // Then toggle (add/remove) the .dark-theme class to the body
  document.body.classList.toggle('dark-theme');  
})