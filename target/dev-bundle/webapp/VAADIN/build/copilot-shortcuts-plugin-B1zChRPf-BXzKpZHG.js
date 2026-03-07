import { s as su, o as ol, y, f as O, B as Be, k as Rl, u as uu } from "./indexhtml-Ckkek0fI.js";
import { i as i$1 } from "./base-panel-C9ezFash-BDNmZs8y.js";
import { C as C$1 } from "./icons-x5khyqlC-CIK0PyAw.js";
const v = 'copilot-shortcuts-panel{display:flex;flex-direction:column;padding:var(--space-150)}copilot-shortcuts-panel h3{font:var(--font-xsmall-semibold);margin-bottom:var(--space-100);margin-top:0}copilot-shortcuts-panel h3:not(:first-of-type){margin-top:var(--space-200)}copilot-shortcuts-panel ul{display:flex;flex-direction:column;list-style:none;margin:0;padding:0}copilot-shortcuts-panel ul li{display:flex;align-items:center;gap:var(--space-50);position:relative}copilot-shortcuts-panel ul li:not(:last-of-type):before{border-bottom:1px dashed var(--border-color);content:"";inset:auto 0 0 calc(var(--size-m) + var(--space-50));position:absolute}copilot-shortcuts-panel ul li span:has(svg){align-items:center;display:flex;height:var(--size-m);justify-content:center;width:var(--size-m)}copilot-shortcuts-panel .kbds{margin-inline-start:auto}copilot-shortcuts-panel kbd{align-items:center;border:1px solid var(--border-color);border-radius:var(--radius-2);box-sizing:border-box;display:inline-flex;font-family:var(--font-family);font-size:var(--font-size-1);line-height:var(--line-height-1);padding:0 var(--space-50)}', u = window.Vaadin.copilot.tree;
if (!u)
  throw new Error("Tried to access copilot tree before it was initialized.");
var w = (s, l, h, p) => {
  for (var o = l, n = s.length - 1, r; n >= 0; n--)
    (r = s[n]) && (o = r(o) || o);
  return o;
};
let d = class extends i$1 {
  constructor() {
    super(), this.onTreeUpdated = () => {
      this.requestUpdate();
    };
  }
  connectedCallback() {
    super.connectedCallback(), y.on("copilot-tree-created", this.onTreeUpdated);
  }
  disconnectedCallback() {
    super.disconnectedCallback(), y.off("copilot-tree-created", this.onTreeUpdated);
  }
  render() {
    const s = u.hasFlowComponents();
    return Be`<style>
        ${v}
      </style>
      <h3>Global</h3>
      <ul>
        <li>
          <span>${C$1.vaadinLogo}</span>
          <span>Copilot</span>
          ${t(uu.toggleCopilot)}
        </li>
        <li>
          <span>${C$1.terminal}</span>
          <span>Command window</span>
          ${t(uu.toggleCommandWindow)}
        </li>
        <li>
          <span>${C$1.flipBack}</span>
          <span>Undo</span>
          ${t(uu.undo)}
        </li>
        <li>
          <span>${C$1.flipForward}</span>
          <span>Redo</span>
          ${t(uu.redo)}
        </li>
      </ul>
      <h3>Selected component</h3>
      <ul>
        <li>
          <span>${C$1.fileCodeAlt}</span>
          <span>Go to source</span>
          ${t(uu.goToSource)}
        </li>
        ${s ? Be`<li>
              <span>${C$1.code}</span>
              <span>Go to attach source</span>
              ${t(uu.goToAttachSource)}
            </li>` : O}
        <li>
          <span>${C$1.copy}</span>
          <span>Copy</span>
          ${t(uu.copy)}
        </li>
        <li>
          <span>${C$1.clipboard}</span>
          <span>Paste</span>
          ${t(uu.paste)}
        </li>
        <li>
          <span>${C$1.copyAlt}</span>
          <span>Duplicate</span>
          ${t(uu.duplicate)}
        </li>
        <li>
          <span>${C$1.userUp}</span>
          <span>Select parent</span>
          ${t(uu.selectParent)}
        </li>
        <li>
          <span>${C$1.userLeft}</span>
          <span>Select previous sibling</span>
          ${t(uu.selectPreviousSibling)}
        </li>
        <li>
          <span>${C$1.userRight}</span>
          <span>Select first child / next sibling</span>
          ${t(uu.selectNextSibling)}
        </li>
        <li>
          <span>${C$1.trash}</span>
          <span>Delete</span>
          ${t(uu.delete)}
        </li>
      </ul>`;
  }
};
d = w([
  ol("copilot-shortcuts-panel")
], d);
function t(s) {
  return Be`<span class="kbds">${Rl(s)}</span>`;
}
const x = su({
  header: "Keyboard Shortcuts",
  tag: "copilot-shortcuts-panel",
  width: 400,
  height: 550,
  floatingPosition: {
    top: 50,
    left: 50
  }
}), C = {
  init(s) {
    s.addPanel(x);
  }
};
window.Vaadin.copilot.plugins.push(C);
