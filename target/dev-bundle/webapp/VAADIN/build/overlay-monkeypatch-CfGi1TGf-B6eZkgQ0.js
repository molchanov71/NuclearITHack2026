import { i as he } from "./indexhtml-Ckkek0fI.js";
/**
 * @license
 * Copyright 2017 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */
const d = (e, a, t) => (t.configurable = true, t.enumerable = true, Reflect.decorate && typeof a != "object" && Object.defineProperty(e, a, t), t);
/**
 * @license
 * Copyright 2017 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */
function h(e, a) {
  return (t, o, v) => {
    const i = (l) => {
      var _a;
      return ((_a = l.renderRoot) == null ? void 0 : _a.querySelector(e)) ?? null;
    };
    return d(t, o, { get() {
      return i(this);
    } });
  };
}
function u(e) {
  e.querySelectorAll(
    "vaadin-context-menu, vaadin-menu-bar, vaadin-menu-bar-submenu, vaadin-select, vaadin-combo-box, vaadin-tooltip, vaadin-dialog, vaadin-multi-select-combo-box, vaadin-popover"
  ).forEach((a) => {
    var _a, _b, _c;
    ((_a = a == null ? void 0 : a.$) == null ? void 0 : _a.comboBox) && (a = a.$.comboBox);
    let t = (_b = a.shadowRoot) == null ? void 0 : _b.querySelector(
      `${a.localName}-overlay, ${a.localName}-submenu, vaadin-menu-bar-overlay`
    );
    (t == null ? void 0 : t.localName) === "vaadin-menu-bar-submenu" && (t = t.shadowRoot.querySelector("vaadin-menu-bar-overlay")), t ? t._attachOverlay = n.bind(t) : ((_c = a.$) == null ? void 0 : _c.overlay) && (a.$.overlay._attachOverlay = n.bind(a.$.overlay));
  });
}
function r() {
  return document.querySelector(`${he}main`).shadowRoot;
}
const m = () => Array.from(r().children).filter((a) => a._hasOverlayStackMixin && !a.hasAttribute("closing")).sort((a, t) => a.__zIndex - t.__zIndex || 0), s = (e) => e === m().pop();
function n() {
  const e = this;
  e._placeholder = document.createComment("vaadin-overlay-placeholder"), e.parentNode.insertBefore(e._placeholder, e), r().appendChild(e), e.hasOwnProperty("_last") || Object.defineProperty(e, "_last", {
    // Only returns odd die sides
    get() {
      return s(this);
    }
  }), e.bringToFront(), requestAnimationFrame(() => u(e));
}
export {
  h,
  u
};
