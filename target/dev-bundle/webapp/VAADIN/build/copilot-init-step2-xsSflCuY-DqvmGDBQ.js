var _a, _b;
import { x as al, o as ol, s as su, P as Pc, c as ae, y, z as Gc, F as Fc, b as bt, S as So, R as Re, A as Nc, C as jc, p as p$1, D as Dl, E as Eo, B as Be$1, a as Bc, f as O$1, e as bu, q as un, r as se$1, i as he, H as Kc, I as qc, L as lu, M as de, Q as Qc, U as te$1, V as pc, _ as Hc, a0 as vn, a1 as dt, m as mu, a2 as vo, g as gu, u as uu, a3 as Ws, a4 as du, a5 as mc, a6 as fu, k as Rl, a7 as yc, Y as Yc, a8 as Oo, a9 as eu } from "./indexhtml-Ckkek0fI.js";
import { b, h } from "./state-DDML0umE-B6VJ4oDN.js";
import { h as h$1, u } from "./overlay-monkeypatch-CfGi1TGf-B6eZkgQ0.js";
import { C } from "./icons-x5khyqlC-CIK0PyAw.js";
import { e } from "./early-project-state-CqEloDes-CqEloDes.js";
/**
 * @license
 * Copyright 2017 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */
function Te(e2) {
  return (t, n) => {
    const i = typeof t == "function" ? t : t[n];
    Object.assign(i, e2);
  };
}
var je = Object.defineProperty, _e = Object.getOwnPropertyDescriptor, k = (e2, t, n, i) => {
  for (var o = i > 1 ? void 0 : i ? _e(t, n) : t, s = e2.length - 1, r; s >= 0; s--)
    (r = e2[s]) && (o = (i ? r(t, n, o) : r(o)) || o);
  return i && o && je(t, n, o), o;
};
const F = "data-drag-initial-index", O = "data-drag-final-index";
let P = class extends Dl {
  constructor() {
    super(...arguments), this.position = "right", this.opened = false, this.keepOpen = false, this.resizing = false, this.closingForcefully = false, this.draggingSectionPanel = null, this.documentMouseUpListener = () => {
      this.resizing && y.emit("user-select", { allowSelection: true }), this.resizing = false, p$1.setDrawerResizing(false), this.removeAttribute("resizing");
    }, this.activationAnimationTransitionEndListener = () => {
      this.style.removeProperty("--closing-delay"), this.style.removeProperty("--initial-position"), this.removeEventListener("transitionend", this.activationAnimationTransitionEndListener);
    }, this.resizingMouseMoveListener = (e2) => {
      if (!this.resizing)
        return;
      const { x: t, y: n } = e2;
      e2.stopPropagation(), e2.preventDefault(), requestAnimationFrame(() => {
        let i;
        if (this.position === "right") {
          const o = document.body.clientWidth - t;
          this.style.setProperty("--size", `${o}px`), de.saveDrawerSize(this.position, o), i = { width: o };
        } else if (this.position === "left") {
          const o = t;
          this.style.setProperty("--size", `${o}px`), de.saveDrawerSize(this.position, o), i = { width: o };
        } else if (this.position === "bottom") {
          const o = document.body.clientHeight - n;
          this.style.setProperty("--size", `${o}px`), de.saveDrawerSize(this.position, o), i = { height: o };
        }
        ae.panels.filter((o) => !o.floating && o.panel === this.position).forEach((o) => {
          ae.updatePanel(o.tag, i);
        });
      });
    }, this.sectionPanelDraggingStarted = (e2, t) => {
      this.draggingSectionPanel = e2, y.emit("user-select", { allowSelection: false }), this.draggingSectionPointerStartY = t.clientY, e2.toggleAttribute("dragging", true), e2.style.zIndex = "1000", Array.from(this.querySelectorAll("copilot-section-panel-wrapper")).forEach((n, i) => {
        n.setAttribute(F, `${i}`);
      }), document.addEventListener("mousemove", this.sectionPanelDragging), document.addEventListener("mouseup", this.sectionPanelDraggingFinished);
    }, this.sectionPanelDragging = (e2) => {
      if (!this.draggingSectionPanel)
        return;
      const { clientX: t, clientY: n } = e2;
      if (!Qc(this.getBoundingClientRect(), t, n)) {
        this.cleanUpDragging();
        return;
      }
      const i = n - this.draggingSectionPointerStartY;
      this.draggingSectionPanel.style.transform = `translateY(${i}px)`, this.updateSectionPanelPositionsWhileDragging();
    }, this.sectionPanelDraggingFinished = () => {
      if (!this.draggingSectionPanel)
        return;
      y.emit("user-select", { allowSelection: true });
      const e2 = this.getAllPanels().filter(
        (t) => {
          var _a2;
          return t.hasAttribute(O) && ((_a2 = t.panelInfo) == null ? void 0 : _a2.panelOrder) !== Number.parseInt(t.getAttribute(O), 10);
        }
      ).map((t) => ({
        tag: t.panelTag,
        order: Number.parseInt(t.getAttribute(O), 10)
      }));
      this.cleanUpDragging(), ae.updateOrders(e2), document.removeEventListener("mouseup", this.sectionPanelDraggingFinished), document.removeEventListener("mousemove", this.sectionPanelDragging);
    }, this.updateSectionPanelPositionsWhileDragging = () => {
      const e2 = this.draggingSectionPanel.getBoundingClientRect().height;
      this.getAllPanels().sort((t, n) => {
        const i = t.getBoundingClientRect(), o = n.getBoundingClientRect(), s = (i.top + i.bottom) / 2, r = (o.top + o.bottom) / 2;
        return s - r;
      }).forEach((t, n) => {
        var _a2;
        if (t.setAttribute(O, `${n}`), t.panelTag !== ((_a2 = this.draggingSectionPanel) == null ? void 0 : _a2.panelTag)) {
          const i = Number.parseInt(t.getAttribute(F), 10);
          i > n ? t.style.transform = `translateY(${-e2}px)` : i < n ? t.style.transform = `translateY(${e2}px)` : t.style.removeProperty("transform");
        }
      });
    };
  }
  static get styles() {
    return [
      te$1(pc),
      al`
        :host {
          --size: 350px;
          --min-size: 20%;
          --max-size: 80%;
          --default-content-height: 300px;
          --transition-duration: var(--duration-2);
          --opening-delay: var(--duration-2);
          --closing-delay: var(--duration-3);
          --hover-size: 18px;
          --initial-position: 0px;
          position: absolute;
          z-index: var(--z-index-drawer);
          transition: translate var(--transition-duration) var(--closing-delay);
        }

        :host([no-transition]),
        :host([no-transition]) .container {
          transition: none;
          -webkit-transition: none;
          -moz-transition: none;
          -o-transition: none;
        }

        :host(:is([position='left'], [position='right'])) {
          width: var(--size);
          min-width: var(--min-size);
          max-width: var(--max-size);
          top: 0;
          bottom: 0;
        }

        :host([position='left']) {
          left: var(--initial-position);
          translate: calc(-100% + var(--hover-size)) 0%;
          padding-right: var(--hover-size);
        }

        :host([position='right']) {
          right: var(--initial-position);
          translate: calc(100% - var(--hover-size)) 0%;
          padding-left: var(--hover-size);
        }

        :host([position='bottom']) {
          height: var(--size);
          min-height: var(--min-size);
          max-height: var(--max-size);
          bottom: var(--initial-position);
          left: 0;
          right: 0;
          translate: 0% calc(100% - var(--hover-size));
          padding-top: var(--hover-size);
        }

        /* The visible container. Needed to have extra space for hover and resize handle outside it. */

        .container {
          display: flex;
          flex-direction: column;
          box-sizing: border-box;
          height: 100%;
          background: var(--background-color);
          -webkit-backdrop-filter: var(--surface-backdrop-filter);
          backdrop-filter: var(--surface-backdrop-filter);
          overflow-y: auto;
          overflow-x: hidden;
          box-shadow: var(--surface-box-shadow-2);
          transition:
            opacity var(--transition-duration) var(--closing-delay),
            visibility calc(var(--transition-duration) * 2) var(--closing-delay);
          opacity: 0;
          /* For accessibility (restored when open) */
          visibility: hidden;
        }

        :host([position='left']) .container {
          border-right: 1px solid var(--surface-border-color);
        }

        :host([position='right']) .container {
          border-left: 1px solid var(--surface-border-color);
        }

        :host([position='bottom']) .container {
          border-top: 1px solid var(--surface-border-color);
        }

        /* Opened state */

        :host(:is([opened], [keepopen])) {
          translate: 0% 0%;
          transition-delay: var(--opening-delay);
          z-index: var(--z-index-opened-drawer);
        }

        :host(:is([opened], [keepopen])) .container {
          transition-delay: var(--opening-delay);
          visibility: visible;
          opacity: 1;
        }

        .drawer-indicator {
          align-items: center;
          border-radius: 9999px;
          box-shadow: inset 0 0 0 1px hsl(0 0% 0% / 0.2);
          color: white;
          display: flex;
          height: 1.75rem;
          justify-content: center;
          overflow: hidden;
          opacity: 1;
          position: absolute;
          transition-delay: 0.5s;
          transition-duration: 0.2s;
          transition-property: opacity;
          width: 1.75rem;
        }

        .drawer-indicator::before {
          animation: 5s swirl linear infinite;
          animation-play-state: running;
          background-image:
            radial-gradient(circle at 50% -10%, hsl(221 100% 55% / 0.6) 0%, transparent 60%),
            radial-gradient(circle at 25% 40%, hsl(303 71% 64%) 0%, transparent 70%),
            radial-gradient(circle at 80% 10%, hsla(262, 38%, 9%, 0.5) 0%, transparent 80%),
            radial-gradient(circle at 110% 50%, hsla(147, 100%, 77%, 1) 20%, transparent 100%);
          content: '';
          inset: 0;
          opacity: 1;
          position: absolute;
          transition: opacity 0.5s;
        }
        :host([attention-required]) .drawer-indicator::before {
          background-image:
            radial-gradient(circle at 50% -10%, hsl(0deg 100% 55% / 60%) 0%, transparent 60%),
            radial-gradient(circle at 25% 40%, hsl(0deg 71% 64%) 0%, transparent 70%),
            radial-gradient(circle at 80% 10%, hsl(0deg 38% 9% / 50%) 0%, transparent 80%),
            radial-gradient(circle at 110% 50%, hsl(0deg 100% 77%) 20%, transparent 100%);
        }
        :host([opened]) .drawer-indicator {
          opacity: 0;
          transition-delay: 0s;
        }

        .drawer-indicator svg {
          height: 0.75rem;
          width: 0.75rem;
          z-index: 1;
        }

        :host([position='right']) .drawer-indicator {
          left: 0.25rem;
          top: calc(50% - 0.875rem);
        }

        :host([position='right']) .drawer-indicator svg {
          margin-inline-start: -0.625rem;
          transform: rotate(-90deg);
        }

        :host([position='left']) .drawer-indicator {
          right: 0.25rem;
          top: calc(50% - 0.875rem);
        }

        :host([position='left']) .drawer-indicator svg {
          margin-inline-end: -0.625rem;
          transform: rotate(90deg);
        }

        :host([position='bottom']) .drawer-indicator {
          left: calc(50% - 0.875rem);
          top: 0.25rem;
        }

        :host([position='bottom']) .drawer-indicator svg {
          margin-top: -0.625rem;
        }

        .overflow-indicator {
          align-items: center;
          border-radius: 9999px;
          bottom: -0.875rem;
          box-shadow: inset 0 0 0 1px hsl(0 0% 0% / 0.2);
          color: white;
          display: flex;
          height: 1.75rem;
          justify-content: center;
          left: calc(50% - 0.875rem);
          overflow: hidden;
          opacity: 0;
          position: absolute;
          transition: opacity 0.2s;
          width: 1.75rem;
        }

        .overflow-indicator::after {
          background: hsl(0 0% 0% / 0.5);
          border-radius: 9999px;
          content: '';
          inset: 2px;
          opacity: 1;
          position: absolute;
        }

        .overflow-indicator::before {
          animation: 5s swirl linear infinite;
          animation-play-state: running;
          background-image:
            radial-gradient(circle at 50% -10%, hsl(221 100% 55% / 0.6) 0%, transparent 60%),
            radial-gradient(circle at 25% 40%, hsl(303 71% 64%) 0%, transparent 70%),
            radial-gradient(circle at 80% 10%, hsla(262, 38%, 9%, 0.5) 0%, transparent 80%),
            radial-gradient(circle at 110% 50%, hsla(147, 100%, 77%, 1) 20%, transparent 100%);
          content: '';
          inset: 0;
          opacity: 1;
          position: absolute;
          transition: opacity 0.5s;
        }

        .overflow-indicator svg {
          height: 0.75rem;
          margin-top: -0.625rem;
          width: 0.75rem;
          z-index: 1;
        }

        :host([position='left']) [canscroll] .overflow-indicator,
        :host([position='right']) [canscroll] .overflow-indicator {
          opacity: 1;
        }

        .resize {
          position: absolute;
          z-index: 10;
          inset: 0;
        }

        :host(:is([position='left'], [position='right'])) .resize {
          width: var(--hover-size);
          cursor: col-resize;
        }

        :host([position='left']) .resize {
          left: auto;
          right: calc(var(--hover-size) * 0.5);
        }

        :host([position='right']) .resize {
          right: auto;
          left: calc(var(--hover-size) * 0.5);
        }

        :host([position='bottom']) .resize {
          height: var(--hover-size);
          bottom: auto;
          top: calc(var(--hover-size) * 0.5);
          cursor: row-resize;
        }

        :host([resizing]) .container {
          /* vaadin-grid (used in the outline) blocks the mouse events */
          pointer-events: none;
        }

        /* Visual indication of the drawer */

        :host::before {
          content: '';
          position: absolute;
          pointer-events: none;
          z-index: -1;
          inset: var(--hover-size);
          transition: opacity var(--transition-duration) var(--closing-delay);
        }

        :host([document-hidden])::before {
          animation: none;
        }

        :host([document-hidden]) .drawer-indicator {
          -webkit-filter: grayscale(100%); /* Chrome, Safari, Opera */
          filter: grayscale(100%);
        }

        :host(:is([opened], [keepopen]))::before {
          transition-delay: var(--opening-delay);
          opacity: 0;
        }

        .hasmoreContainer {
          height: 100%;
          position: relative;
        }
      `
    ];
  }
  connectedCallback() {
    super.connectedCallback(), this.reaction(
      () => ae.panels,
      () => this.requestUpdate()
    ), this.reaction(
      () => p$1.operationInProgress,
      (t) => {
        t === lu.DragAndDrop && !this.opened && !this.keepOpen ? this.style.setProperty("pointer-events", "none") : this.style.setProperty("pointer-events", "auto");
      }
    ), this.reaction(
      () => ae.getAttentionRequiredPanelConfiguration(),
      () => {
        const t = ae.getAttentionRequiredPanelConfiguration();
        t && !t.floating && this.toggleAttribute(Hc, t.panel === this.position);
      }
    ), this.reaction(
      () => p$1.active,
      () => {
        if (!p$1.active || !vn.isActivationAnimation() || p$1.activatedFrom === "restore" || p$1.activatedFrom === "test")
          return;
        const t = ae.getAttentionRequiredPanelConfiguration();
        t && !t.floating && t.panel === this.position || (this.addEventListener("transitionend", this.activationAnimationTransitionEndListener), this.toggleAttribute("no-transition", true), this.opened = true, this.style.setProperty("--closing-delay", "var(--duration-1)"), this.style.setProperty("--initial-position", "calc(-1 * (max(var(--size), var(--min-size)) * 1) / 3)"), requestAnimationFrame(() => {
          this.toggleAttribute("no-transition", false), this.opened = false;
        }));
      }
    ), document.addEventListener("mouseup", this.documentMouseUpListener);
    const e2 = de.getDrawerSize(this.position);
    e2 && this.style.setProperty("--size", `${e2}px`), document.addEventListener("mousemove", this.resizingMouseMoveListener), this.addEventListener("mouseenter", this.mouseEnterListener), y.on("document-activation-change", (t) => {
      this.toggleAttribute("document-hidden", !t.detail.active);
    });
  }
  firstUpdated(e2) {
    super.firstUpdated(e2), requestAnimationFrame(() => this.toggleAttribute("no-transition", false)), this.resizeElement.addEventListener("mousedown", (t) => {
      t.button === 0 && (this.resizing = true, p$1.setDrawerResizing(true), this.setAttribute("resizing", ""), y.emit("user-select", { allowSelection: false }));
    });
  }
  updated(e2) {
    super.updated(e2), e2.has("opened") && this.opened && this.hasAttribute(Hc) && (this.removeAttribute(Hc), ae.clearAttention()), this.updateScrollable();
  }
  disconnectedCallback() {
    super.disconnectedCallback(), document.removeEventListener("mousemove", this.resizingMouseMoveListener), document.removeEventListener("mouseup", this.documentMouseUpListener), this.removeEventListener("mouseenter", this.mouseEnterListener);
  }
  /**
   * Cleans up attributes/styles etc... for dragging operations
   * @private
   */
  cleanUpDragging() {
    this.draggingSectionPanel && (p$1.setSectionPanelDragging(false), this.draggingSectionPanel.style.zIndex = "", Array.from(this.querySelectorAll("copilot-section-panel-wrapper")).forEach((e2) => {
      e2.style.removeProperty("transform"), e2.removeAttribute(O), e2.removeAttribute(F);
    }), this.draggingSectionPanel.removeAttribute("dragging"), this.draggingSectionPanel = null);
  }
  getAllPanels() {
    return Array.from(this.querySelectorAll("copilot-section-panel-wrapper"));
  }
  /**
   * Closes the drawer and disables mouse enter event for a while.
   */
  forceClose() {
    this.closingForcefully = true, this.opened = false, setTimeout(() => {
      this.closingForcefully = false;
    }, 0.5);
  }
  mouseEnterListener(e2) {
    if (this.closingForcefully || p$1.sectionPanelResizing)
      return;
    document.querySelector("copilot-main").shadowRoot.querySelector("copilot-drawer-panel[opened]") || (this.opened = true);
  }
  render() {
    return Be$1`
      <div class="hasmoreContainer">
        <div class="container" @scroll=${this.updateScrollable}>
          <slot></slot>
        </div>
        <div class="overflow-indicator">${C.chevronDown}</div>
      </div>
      <div class="resize"></div>
      <div class="drawer-indicator">${C.chevronUp}</div>
    `;
  }
  updateScrollable() {
    this.hasmoreContainer.toggleAttribute(
      "canscroll",
      this.container.scrollHeight - this.container.scrollTop - this.container.clientHeight > 10
    );
  }
};
k([
  h({ reflect: true, attribute: true })
], P.prototype, "position", 2);
k([
  h({ reflect: true, type: Boolean })
], P.prototype, "opened", 2);
k([
  h({ reflect: true, type: Boolean })
], P.prototype, "keepOpen", 2);
k([
  h$1(".container")
], P.prototype, "container", 2);
k([
  h$1(".hasmoreContainer")
], P.prototype, "hasmoreContainer", 2);
k([
  h$1(".resize")
], P.prototype, "resizeElement", 2);
k([
  Te({ passive: true })
], P.prototype, "updateScrollable", 1);
P = k([
  ol("copilot-drawer-panel")
], P);
var He = Object.defineProperty, Be = Object.getOwnPropertyDescriptor, ge = (e2, t, n, i) => {
  for (var o = i > 1 ? void 0 : i ? Be(t, n) : t, s = e2.length - 1, r; s >= 0; s--)
    (r = e2[s]) && (o = (i ? r(t, n, o) : r(o)) || o);
  return i && o && He(t, n, o), o;
};
let Y = class extends dt {
  constructor() {
    super(...arguments), this.checked = false;
  }
  static get styles() {
    return al`
      .switch {
        display: inline-flex;
        align-items: center;
        gap: var(--space-100);
      }

      .switch input {
        height: 0;
        opacity: 0;
        width: 0;
      }

      .slider {
        background-color: var(--gray-300);
        border-radius: 9999px;
        cursor: pointer;
        inset: 0;
        position: absolute;
        transition: 0.4s;
        height: 0.75rem;
        position: relative;
        width: 1.5rem;
        min-width: 1.5rem;
      }

      .slider:before {
        background-color: white;
        border-radius: 50%;
        bottom: 1px;
        content: '';
        height: 0.625rem;
        left: 1px;
        position: absolute;
        transition: 0.4s;
        width: 0.625rem;
      }

      input:checked + .slider {
        background-color: var(--selection-color);
      }

      input:checked + .slider:before {
        transform: translateX(0.75rem);
      }

      label:has(input:focus) {
        outline: 2px solid var(--selection-color);
        outline-offset: 2px;
      }
    `;
  }
  render() {
    return Be$1`
      <label class="switch">
        <input
          class="feature-toggle"
          id="feature-toggle-${this.id}"
          type="checkbox"
          ?checked="${this.checked}"
          @change=${(e2) => {
      e2.preventDefault(), this.checked = e2.target.checked, this.dispatchEvent(new CustomEvent("on-change"));
    }} />
        <span class="slider"></span>
        ${this.title}
      </label>
    `;
  }
  //  @change=${(e: InputEvent) => this.toggleFeatureFlag(e, feature)}
};
ge([
  h({ reflect: true, type: Boolean })
], Y.prototype, "checked", 2);
Y = ge([
  ol("copilot-toggle-button")
], Y);
class Ue {
  constructor() {
    this.offsetX = 0, this.offsetY = 0;
  }
  draggingStarts(t, n) {
    this.offsetX = n.clientX - t.getBoundingClientRect().left, this.offsetY = n.clientY - t.getBoundingClientRect().top;
  }
  dragging(t, n) {
    const i = n.clientX, o = n.clientY, s = i - this.offsetX, r = i - this.offsetX + t.getBoundingClientRect().width, h2 = o - this.offsetY, u2 = o - this.offsetY + t.getBoundingClientRect().height;
    return this.adjust(t, s, h2, r, u2);
  }
  adjust(t, n, i, o, s) {
    let r, h2, u2, v;
    const y2 = document.documentElement.getBoundingClientRect().width, J = document.documentElement.getBoundingClientRect().height;
    return (o + n) / 2 < y2 / 2 ? (t.style.setProperty("--left", `${n}px`), t.style.setProperty("--right", ""), v = void 0, r = Math.max(0, n)) : (t.style.removeProperty("--left"), t.style.setProperty("--right", `${y2 - o}px`), r = void 0, v = Math.max(0, y2 - o)), (i + s) / 2 < J / 2 ? (t.style.setProperty("--top", `${i}px`), t.style.setProperty("--bottom", ""), u2 = void 0, h2 = Math.max(0, i)) : (t.style.setProperty("--top", ""), t.style.setProperty("--bottom", `${J - s}px`), h2 = void 0, u2 = Math.max(0, J - s)), {
      left: r,
      right: v,
      top: h2,
      bottom: u2
    };
  }
  anchor(t) {
    const { left: n, top: i, bottom: o, right: s } = t.getBoundingClientRect();
    return this.adjust(t, n, i, s, o);
  }
  anchorLeftTop(t) {
    const { left: n, top: i } = t.getBoundingClientRect();
    return t.style.setProperty("--left", `${n}px`), t.style.setProperty("--right", ""), t.style.setProperty("--top", `${i}px`), t.style.setProperty("--bottom", ""), {
      left: n,
      top: i
    };
  }
}
const x = new Ue();
var Ne = Object.defineProperty, Je = Object.getOwnPropertyDescriptor, R = (e2, t, n, i) => {
  for (var o = i > 1 ? void 0 : i ? Je(t, n) : t, s = e2.length - 1, r; s >= 0; s--)
    (r = e2[s]) && (o = (i ? r(t, n, o) : r(o)) || o);
  return i && o && Ne(t, n, o), o;
};
const ee = "https://github.com/JetBrains/JetBrainsRuntime/releases";
function Ve(e2, t) {
  if (!t)
    return true;
  const [n, i, o] = t.split(".").map((u2) => parseInt(u2)), [s, r, h2] = e2.split(".").map((u2) => parseInt(u2));
  if (n < s)
    return true;
  if (n === s) {
    if (i < r)
      return true;
    if (i === r)
      return o < h2;
  }
  return false;
}
const te = "Download complete";
let w = class extends Dl {
  constructor() {
    super(), this.javaPluginSectionOpened = false, this.hotswapSectionOpened = false, this.hotswapTab = "hotswapagent", this.downloadStatusMessages = [], this.downloadProgress = 0, this.onDownloadStatusUpdate = this.downloadStatusUpdate.bind(this), this.reaction(
      () => [p$1.jdkInfo, p$1.idePluginState],
      () => {
        p$1.idePluginState && (!p$1.idePluginState.ide || !p$1.idePluginState.active ? this.javaPluginSectionOpened = true : (!(/* @__PURE__ */ new Set(["vscode", "intellij"])).has(p$1.idePluginState.ide) || !p$1.idePluginState.active) && (this.javaPluginSectionOpened = false)), p$1.jdkInfo && Eo() !== "success" && (this.hotswapSectionOpened = true);
      },
      { fireImmediately: true }
    );
  }
  connectedCallback() {
    super.connectedCallback(), y.on("set-up-vs-code-hotswap-status", this.onDownloadStatusUpdate);
  }
  disconnectedCallback() {
    super.disconnectedCallback(), y.off("set-up-vs-code-hotswap-status", this.onDownloadStatusUpdate);
  }
  render() {
    var _a2, _b2, _c, _d;
    const e2 = {
      intellij: ((_a2 = p$1.idePluginState) == null ? void 0 : _a2.ide) === "intellij",
      vscode: ((_b2 = p$1.idePluginState) == null ? void 0 : _b2.ide) === "vscode",
      eclipse: ((_c = p$1.idePluginState) == null ? void 0 : _c.ide) === "eclipse",
      idePluginInstalled: !!((_d = p$1.idePluginState) == null ? void 0 : _d.active)
    };
    return Be$1`
      <div part="container">${this.renderPluginSection(e2)} ${this.renderHotswapSection(e2)}</div>
      <div part="footer">
        <vaadin-button
          id="close"
          @click="${() => ae.updatePanel(G.tag, { floating: false })}"
          >Close
        </vaadin-button>
      </div>
    `;
  }
  renderPluginSection(e2) {
    let t = "";
    e2.intellij ? t = "IntelliJ" : e2.vscode ? t = "VS Code" : e2.eclipse && (t = "Eclipse");
    let n, i;
    e2.vscode || e2.intellij ? e2.idePluginInstalled ? (n = `Plugin for ${t} installed`, i = this.renderPluginInstalledContent()) : (n = `Plugin for ${t} not installed`, i = this.renderPluginIsNotInstalledContent(e2)) : e2.eclipse ? (n = "Eclipse development workflow is not supported yet", i = this.renderEclipsePluginContent()) : (n = "No IDE found", i = this.renderNoIdePluginContent());
    const o = e2.idePluginInstalled ? C.checkCircle : C.alertTriangle;
    return Be$1`
      <details
        part="panel"
        .open=${this.javaPluginSectionOpened}
        @toggle=${(s) => {
      Bc(() => {
        this.javaPluginSectionOpened = s.target.open;
      });
    }}>
        <summary part="header">
          <span class="icon ${e2.idePluginInstalled ? "success" : "warning"}">${o}</span>
          <div>${n}</div>
        </summary>
        <div part="content">${i}</div>
      </details>
    `;
  }
  renderNoIdePluginContent() {
    return Be$1`
      <div>
        <div>We could not detect an IDE</div>
        ${this.recommendSupportedPlugin()}
      </div>
    `;
  }
  renderEclipsePluginContent() {
    return Be$1`
      <div>
        <div>Eclipse workflow environment is not supported yet.</div>
        ${this.recommendSupportedPlugin()}
      </div>
    `;
  }
  recommendSupportedPlugin() {
    return Be$1`<p>
      Please use <a href="https://code.visualstudio.com">Visual Studio Code</a> or
      <a href="https://www.jetbrains.com/idea">IntelliJ IDEA</a> for better development experience
    </p>`;
  }
  renderPluginInstalledContent() {
    return Be$1` <p>You have a running plugin. Enjoy your awesome development workflow!</p> `;
  }
  renderPluginIsNotInstalledContent(e2) {
    let t = null, n = "Install from Marketplace";
    return e2.intellij ? (t = Kc, n = "Install from JetBrains Marketplace") : e2.vscode && (t = qc, n = "Install from VSCode Marketplace"), Be$1`
      <div>
        <div>Install the Vaadin IDE Plugin to ensure a smooth development workflow</div>
        <p>
          Installing the plugin is not required, but strongly recommended.<br />Some Vaadin Copilot functionality, such
          as undo, will not function optimally without the plugin.
        </p>
        ${t ? Be$1` <div>
              <vaadin-button
                @click="${() => {
      window.open(t, "_blank");
    }}"
                >${n}
                <vaadin-icon icon="vaadin:external-link"></vaadin-icon>
              </vaadin-button>
            </div>` : O$1}
      </div>
    `;
  }
  renderHotswapSection(e2) {
    const { jdkInfo: t } = p$1;
    if (!t)
      return O$1;
    const n = Eo(), i = bu();
    let o, s, r;
    return n === "success" ? (o = C.checkCircle, r = "Java Hotswap is enabled") : n === "warning" ? (o = C.alertTriangle, r = "Java Hotswap is not enabled") : n === "error" && (o = C.alertTriangle, r = "Java Hotswap is partially enabled"), this.hotswapTab === "jrebel" ? t.jrebel ? s = this.renderJRebelInstalledContent() : s = this.renderJRebelNotInstalledContent() : e2.intellij ? s = this.renderHotswapAgentIntelliJPluginContent() : s = this.renderHotswapAgentNotInstalledContent(e2), Be$1` <details
      part="panel"
      .open=${this.hotswapSectionOpened}
      @toggle=${(h2) => {
      Bc(() => {
        this.hotswapSectionOpened = h2.target.open;
      });
    }}>
      <summary part="header">
        <span class="icon ${n}">${o}</span>
        <div>${r}</div>
      </summary>
      <div part="content">
        ${i !== "none" ? Be$1`${i === "jrebel" ? this.renderJRebelInstalledContent() : this.renderHotswapAgentInstalledContent()}` : Be$1`
            <div class="tabs" role="tablist">
              <button
                aria-selected="${this.hotswapTab === "hotswapagent" ? "true" : "false"}"
                class="tab"
                role="tab"
                @click=${() => {
      this.hotswapTab = "hotswapagent";
    }}>
                Hotswap Agent
              </button>
              <button
                aria-selected="${this.hotswapTab === "jrebel" ? "true" : "false"}"
                class="tab"
                role="tab"
                @click=${() => {
      this.hotswapTab = "jrebel";
    }}>
                JRebel
              </button>
            </div>
            <div part="content">${s}</div>
            </div>
            </details>
          `}
      </div>
    </details>`;
  }
  renderJRebelNotInstalledContent() {
    return Be$1`
      <div>
        <a href="https://www.jrebel.com">JRebel ${C.share}</a> is a commercial hotswap solution. Vaadin detects the
        JRebel Agent and automatically reloads the application in the browser after the Java changes have been
        hotpatched.
        <p>
          Go to
          <a href="https://www.jrebel.com/products/jrebel/learn" target="_blank" rel="noopener noreferrer">
            https://www.jrebel.com/products/jrebel/learn ${C.share}</a
          >
          to get started
        </p>
      </div>
    `;
  }
  renderHotswapAgentNotInstalledContent(e2) {
    const t = [
      this.renderJavaRunningInDebugModeSection(),
      this.renderHotswapAgentJdkSection(e2),
      this.renderInstallHotswapAgentJdkSection(e2),
      this.renderHotswapAgentVersionSection(),
      this.renderHotswapAgentMissingArgParam(e2)
    ];
    return Be$1` <div part="hotswap-agent-section-container">${t}</div> `;
  }
  renderHotswapAgentIntelliJPluginContent() {
    const t = Eo() === "success";
    return Be$1`
      <div part="hotswap-agent-section-container">
        <div class="inner-section">
          <details class="inner" .open="${!t}">
            <summary>
              <span class="icon ${t ? "success" : "warning"}"
                >${t ? C.checkCircle : C.alertTriangle}</span
              >
              <div>Use 'Debug using Hotswap Agent' launch configuration</div>
            </summary>
            <div class="hint">
              Vaadin IntelliJ plugin offers launch mode that does not require any manual configuration!
              <p>
                In order to run recommended launch configuration, you should click three dots right next to Debug button
                and select <code>Debug using Hotswap Agent</code> option.
              </p>
            </div>
          </details>
        </div>
      </div>
    `;
  }
  renderJavaRunningInDebugModeSection() {
    var _a2;
    const e2 = (_a2 = p$1.jdkInfo) == null ? void 0 : _a2.runningInJavaDebugMode;
    return Be$1`
      <div class="inner-section">
        <details class="inner" .open="${!e2}">
          <summary>
            <span class="icon">${e2 ? C.checkCircle : C.alertTriangle}</span>
            <div>Run Java in debug mode</div>
          </summary>
          <div class="hint">Start the application in debug mode in the IDE</div>
        </details>
      </div>
    `;
  }
  renderHotswapAgentMissingArgParam(e2) {
    var _a2, _b2;
    const t = ((_a2 = p$1.jdkInfo) == null ? void 0 : _a2.runningWitHotswap) && ((_b2 = p$1.jdkInfo) == null ? void 0 : _b2.runningWithExtendClassDef);
    return Be$1`
      <div class="inner-section">
        <details class="inner" .open="${!t}">
          <summary>
            <span class="icon ${t ? "success" : "warning"}"
              >${t ? C.checkCircle : C.alertTriangle}</span
            >
            <div>Enable HotswapAgent</div>
          </summary>
          <div class="hint">
            <ul>
              ${e2.intellij ? Be$1`<li>Launch as mentioned in the previous step</li>` : O$1}
              ${e2.intellij ? Be$1`<li>
                    To manually configure IntelliJ, add the
                    <code>-XX:HotswapAgent=fatjar -XX:+AllowEnhancedClassRedefinition -XX:+UpdateClasses</code> JVM
                    arguments when launching the application
                  </li>` : Be$1`<li>
                    Add the
                    <code>-XX:HotswapAgent=fatjar -XX:+AllowEnhancedClassRedefinition -XX:+UpdateClasses</code> JVM
                    arguments when launching the application
                  </li>`}
            </ul>
          </div>
        </details>
      </div>
    `;
  }
  renderHotswapAgentJdkSection(e2) {
    var _a2, _b2, _c;
    const t = (_a2 = p$1.jdkInfo) == null ? void 0 : _a2.extendedClassDefCapable, n = ((_b2 = this.downloadStatusMessages) == null ? void 0 : _b2[this.downloadStatusMessages.length - 1]) === te;
    return Be$1`
      <div class="inner-section">
        <details class="inner" .open="${!t}">
          <summary>
            <span class="icon ${t ? "success" : "warning"}"
              >${t ? C.checkCircle : C.alertTriangle}</span
            >
            <div>Run using JetBrains Runtime JDK</div>
          </summary>
          <div class="hint">
            JetBrains Runtime provides much better hotswapping compared to other JDKs.
            <ul>
              ${e2.intellij && Ve("1.3.0", (_c = p$1.idePluginState) == null ? void 0 : _c.version) ? Be$1` <li>Upgrade to the latest IntelliJ plugin</li>` : O$1}
              ${e2.intellij ? Be$1` <li>Launch the application in IntelliJ using "Debug using Hotswap Agent"</li>` : O$1}
              ${e2.vscode ? Be$1` <li>
                    <a href @click="${(i) => this.downloadJetbrainsRuntime(i)}"
                      >Let Copilot download and set up JetBrains Runtime for VS Code</a
                    >
                    ${this.downloadProgress > 0 ? Be$1`<vaadin-progress-bar
                          .value="${this.downloadProgress}"
                          min="0"
                          max="1"></vaadin-progress-bar>` : O$1}
                    <ul>
                      ${this.downloadStatusMessages.map((i) => Be$1`<li>${i}</li>`)}
                      ${n ? Be$1`<h3>Go to VS Code and launch the 'Debug using Hotswap Agent' configuration</h3>` : O$1}
                    </ul>
                  </li>` : O$1}
              <li>
                ${e2.intellij || e2.vscode ? Be$1`If there is a problem, you can manually
                      <a target="_blank" href="${ee}">download JetBrains Runtime JDK</a> and set up
                      your debug configuration to use it.` : Be$1`<a target="_blank" href="${ee}">Download JetBrains Runtime JDK</a> and set up
                      your debug configuration to use it.`}
              </li>
            </ul>
          </div>
        </details>
      </div>
    `;
  }
  renderInstallHotswapAgentJdkSection(e2) {
    var _a2, _b2;
    const t = (_a2 = p$1.jdkInfo) == null ? void 0 : _a2.hotswapAgentFound, n = (_b2 = p$1.jdkInfo) == null ? void 0 : _b2.extendedClassDefCapable;
    return Be$1`
      <div class="inner-section">
        <details class="inner" .open="${!t}">
          <summary>
            <span class="icon ${t ? "success" : "warning"}"
              >${t ? C.checkCircle : C.alertTriangle}</span
            >
            <div>Install HotswapAgent</div>
          </summary>
          <div class="hint">
            Hotswap Agent provides application level support for hot reloading, such as reinitalizing Vaadin @Route or
            @BrowserCallable classes when they are updated
            <ul>
              ${e2.intellij ? Be$1`<li>Launch as mentioned in the previous step</li>` : O$1}
              ${!e2.intellij && !n ? Be$1`<li>First install JetBrains Runtime as mentioned in the step above.</li>` : O$1}
              ${e2.intellij ? Be$1`<li>
                    To manually configure IntelliJ, download HotswapAgent and install the jar file as
                    <code>[JAVA_HOME]/lib/hotswap/hotswap-agent.jar</code> in the JetBrains Runtime JDK. Note that the
                    file must be renamed to exactly match this path.
                  </li>` : Be$1`<li>
                    Download HotswapAgent and install the jar file as
                    <code>[JAVA_HOME]/lib/hotswap/hotswap-agent.jar</code> in the JetBrains Runtime JDK. Note that the
                    file must be renamed to exactly match this path.
                  </li>`}
            </ul>
          </div>
        </details>
      </div>
    `;
  }
  renderHotswapAgentVersionSection() {
    var _a2, _b2, _c, _d;
    if (!((_a2 = p$1.jdkInfo) == null ? void 0 : _a2.hotswapAgentFound))
      return O$1;
    const e2 = (_b2 = p$1.jdkInfo) == null ? void 0 : _b2.hotswapVersionOk, t = (_c = p$1.jdkInfo) == null ? void 0 : _c.hotswapVersion, n = (_d = p$1.jdkInfo) == null ? void 0 : _d.hotswapAgentLocation;
    return Be$1`
      <div class="inner-section">
        <details class="inner" .open="${!e2}">
          <summary>
            <span class="icon ${e2 ? "success" : "warning"}"
              >${e2 ? C.checkCircle : C.alertTriangle}</span
            >
            <div>Hotswap version requires update</div>
          </summary>
          <div class="hint">
            HotswapAgent version ${t} is in use
            <a target="_blank" href="https://github.com/HotswapProjects/HotswapAgent/releases"
              >Download the latest HotswapAgent</a
            >
            and place it in <code>${n}</code>
          </div>
        </details>
      </div>
    `;
  }
  renderJRebelInstalledContent() {
    return Be$1` <div>JRebel is in use. Enjoy your awesome development workflow!</div> `;
  }
  renderHotswapAgentInstalledContent() {
    return Be$1` <div>Hotswap agent is in use. Enjoy your awesome development workflow!</div> `;
  }
  async downloadJetbrainsRuntime(e2) {
    return e2.target.disabled = true, e2.preventDefault(), this.downloadStatusMessages = [], un(`${he}set-up-vs-code-hotswap`, {}, (t) => {
      t.data.error ? (se$1("Error downloading JetBrains runtime", t.data.error), this.downloadStatusMessages = [...this.downloadStatusMessages, "Download failed"]) : this.downloadStatusMessages = [...this.downloadStatusMessages, te];
    });
  }
  downloadStatusUpdate(e2) {
    const t = e2.detail.progress;
    t ? this.downloadProgress = t : this.downloadStatusMessages = [...this.downloadStatusMessages, e2.detail.message];
  }
};
w.NAME = "copilot-development-setup-user-guide";
w.styles = al`
    :host {
      --icon-size: 24px;
      --summary-header-gap: 10px;
      --footer-height: calc(50px + var(--space-150));
      color: var(--color);
    }
    :host code {
      background-color: var(--gray-50);
      font-size: var(--font-size-0);
      display: inline-block;
      margin-top: var(--space-100);
      margin-bottom: var(--space-100);
      user-select: all;
    }

    [part='container'] {
      display: flex;
      flex-direction: column;
      gap: var(--space-150);
      padding: var(--space-150);
      box-sizing: border-box;
      height: calc(100% - var(--footer-height));
      overflow: auto;
    }

    [part='footer'] {
      display: flex;
      justify-content: flex-end;
      height: var(--footer-height);
      padding-left: var(--space-150);
      padding-right: var(--space-150);
    }
    [part='hotswap-agent-section-container'] {
      display: flex;
      flex-direction: column;
      gap: var(--space-100);
    }
    [part='content'] {
      display: flex;
      padding: var(--space-150);
      flex-direction: column;
    }
    div.inner-section div.hint {
      margin-left: calc(var(--summary-header-gap) + var(--icon-size));
      margin-top: var(--space-75);
    }
    details {
      display: flex;
      flex-direction: column;
      box-sizing: border-box;

      & > summary span.icon {
        width: var(--icon-size);
        height: var(--icon-size);
      }
      & > summary span.icon.warning {
        color: var(--lumo-warning-color);
      }
      & > summary span.icon.success {
        color: var(--lumo-success-color);
      }
      & > summary,
      summary::part(header) {
        display: flex;
        flex-direction: row;
        align-items: center;
        cursor: pointer;
        position: relative;
        gap: var(--summary-header-gap);
        font: var(--font);
      }
      summary::after,
      summary::part(header)::after {
        content: '';
        display: block;
        width: 4px;
        height: 4px;
        border-color: var(--color);
        opacity: var(--panel-toggle-opacity, 0.2);
        border-width: 2px;
        border-style: solid solid none none;
        transform: rotate(var(--panel-toggle-angle, -45deg));
        position: absolute;
        right: 15px;
        top: calc(50% - var(--panel-toggle-offset, 2px));
      }
      &:not([open]) {
        --panel-toggle-angle: 135deg;
        --panel-toggle-offset: 4px;
      }
    }
    details[part='panel'] {
      background: var(--card-bg);
      border: var(--card-border);
      border-radius: 4px;
      user-select: none;

      &:has(summary:hover) {
        border-color: var(--accent-color);
      }

      & > summary,
      summary::part(header) {
        padding: 10px 10px;
        padding-right: 25px;
      }

      summary:hover,
      summary::part(header):hover {
        --panel-toggle-opacity: 0.5;
      }

      input[type='checkbox'],
      summary::part(checkbox) {
        margin: 0;
      }

      &:not([open]):hover {
        background: var(--card-hover-bg);
      }

      &[open] {
        background: var(--card-open-bg);
        box-shadow: var(--card-open-shadow);

        & > summary {
          font-weight: bold;
        }
      }
      .tabs {
        border-bottom: 1px solid var(--border-color);
        box-sizing: border-box;
        display: flex;
        height: 2.25rem;
      }

      .tab {
        background: none;
        border: none;
        border-bottom: 1px solid transparent;
        color: var(--color);
        font: var(--font-button);
        height: 2.25rem;
        padding: 0 0.75rem;
      }

      .tab[aria-selected='true'] {
        color: var(--color-high-contrast);
        border-bottom-color: var(--color-high-contrast);
      }

      .tab-content {
        flex: 1 1 auto;
        gap: var(--space-150);
        overflow: auto;
        padding: var(--space-150);
      }
    }
  `;
R([
  b()
], w.prototype, "javaPluginSectionOpened", 2);
R([
  b()
], w.prototype, "hotswapSectionOpened", 2);
R([
  b()
], w.prototype, "hotswapTab", 2);
R([
  b()
], w.prototype, "downloadStatusMessages", 2);
R([
  b()
], w.prototype, "downloadProgress", 2);
w = R([
  ol(w.NAME)
], w);
const G = su({
  header: "Development Workflow",
  tag: Pc,
  width: 800,
  height: 800,
  floatingPosition: {
    top: 50,
    left: 50
  },
  individual: true
}), Fe = {
  init(e2) {
    e2.addPanel(G);
  }
};
window.Vaadin.copilot.plugins.push(Fe);
ae.addPanel(G);
var Xe = (e2, t, n, i) => {
  for (var o = t, s = e2.length - 1, r; s >= 0; s--)
    (r = e2[s]) && (o = r(o) || o);
  return o;
};
let ie = class extends Dl {
  createRenderRoot() {
    return this;
  }
  connectedCallback() {
    super.connectedCallback(), this.classList.add("custom-menu-item");
  }
  render() {
    const e2 = mu(), t = e2.status === "warning" || e2.status === "error";
    return Be$1`
      <div style="flex-grow: 1;">
        <div class="label">Development workflow</div>
        <div class="status ${t ? e2.status : ""}">${e2.message}</div>
      </div>
      ${t ? Be$1`<div class="${e2.status} icon"></div>` : O$1}
    `;
  }
};
ie = Xe([
  ol("copilot-activation-button-development-workflow")
], ie);
var We = (e2, t, n, i) => {
  for (var o = t, s = e2.length - 1, r; s >= 0; s--)
    (r = e2[s]) && (o = r(o) || o);
  return o;
};
let ne = class extends Dl {
  constructor() {
    super(), this.clickListener = this.getClickListener(), this.reaction(
      () => p$1.userInfo,
      () => {
        this.requestUpdate();
      }
    );
  }
  createRenderRoot() {
    return this;
  }
  connectedCallback() {
    super.connectedCallback(), this.classList.add("custom-menu-item"), this.addEventListener("click", this.clickListener);
  }
  disconnectedCallback() {
    super.disconnectedCallback(), this.removeEventListener("click", this.clickListener);
  }
  render() {
    const e2 = this.getStatus();
    return Be$1`
      <div style="flex-grow: 1;">
        <div class="label user">${this.getUsername()}</div>
        ${e2 ? Be$1`<div class="warning status">${e2}</div>` : O$1}
      </div>
      ${this.renderPortrait()} ${this.renderDot()}
    `;
  }
  getClickListener() {
    var _a2;
    return ((_a2 = p$1.userInfo) == null ? void 0 : _a2.validLicense) ? () => window.open("https://vaadin.com/myaccount", "_blank", "noopener") : () => p$1.setLoginCheckActive(true);
  }
  getUsername() {
    var _a2;
    return ((_a2 = p$1.userInfo) == null ? void 0 : _a2.firstName) ? `${p$1.userInfo.firstName} ${p$1.userInfo.lastName}` : "Log in";
  }
  getStatus() {
    var _a2, _b2, _c;
    if (!((_a2 = p$1.userInfo) == null ? void 0 : _a2.validLicense)) {
      if (vo.active) {
        const e2 = Math.round(vo.remainingTimeInMillis / 864e5);
        return `Preview expires in ${e2}${e2 === 1 ? " day" : " days"}`;
      }
      if (vo.expired && !((_b2 = p$1.userInfo) == null ? void 0 : _b2.validLicense))
        return "Preview expired";
      if (!vo.active && !vo.expired && !((_c = p$1.userInfo) == null ? void 0 : _c.validLicense))
        return "No valid license available";
    }
  }
  renderPortrait() {
    var _a2;
    return ((_a2 = p$1.userInfo) == null ? void 0 : _a2.portraitUrl) ? Be$1`<div
        class="portrait"
        style="background-image: url('https://vaadin.com${p$1.userInfo.portraitUrl}')"></div>` : O$1;
  }
  renderDot() {
    var _a2;
    return ((_a2 = p$1.userInfo) == null ? void 0 : _a2.validLicense) ? O$1 : vo.active || vo.expired ? Be$1`<div class="icon warning"></div>` : O$1;
  }
};
ne = We([
  ol("copilot-activation-button-user-info")
], ne);
function f(e2) {
  return ue("vaadin-menu-bar-item", e2);
}
function q(e2) {
  return ue("vaadin-context-menu-item", e2);
}
function ue(e2, t) {
  const n = document.createElement(e2);
  if (t.style && (n.className = t.style), t.icon)
    if (typeof t.icon == "string") {
      const i = document.createElement("vaadin-icon");
      i.setAttribute("icon", t.icon), n.append(i);
    } else
      n.append(Ge(t.icon.strings[0]));
  if (t.label) {
    const i = document.createElement("span");
    i.className = "label", i.innerHTML = t.label, n.append(i);
  } else if (t.component) {
    const i = eu(t.component) ? t.component : document.createElement(t.component);
    n.append(i);
  }
  if (t.hint) {
    const i = document.createElement("span");
    i.className = "hint", i.innerHTML = t.hint, n.append(i);
  }
  return n;
}
function Ge(e2) {
  if (!e2) return null;
  const t = document.createElement("template");
  t.innerHTML = e2;
  const n = t.content.children;
  return n.length === 1 ? n[0] : n;
}
var Ke = Object.defineProperty, Ze = Object.getOwnPropertyDescriptor, N = (e2, t, n, i) => {
  for (var o = i > 1 ? void 0 : i ? Ze(t, n) : t, s = e2.length - 1, r; s >= 0; s--)
    (r = e2[s]) && (o = (i ? r(t, n, o) : r(o)) || o);
  return i && o && Ke(t, n, o), o;
};
const Qe = 8;
let T = class extends Dl {
  constructor() {
    super(...arguments), this.initialMouseDownPosition = null, this.dragging = false, this.items = [], this.mouseDownListener = (e2) => {
      this.initialMouseDownPosition = { x: e2.clientX, y: e2.clientY }, x.draggingStarts(this, e2), document.addEventListener("mousemove", this.documentDraggingMouseMoveEventListener);
    }, this.documentDraggingMouseMoveEventListener = (e2) => {
      if (this.initialMouseDownPosition && !this.dragging) {
        const { clientX: t, clientY: n } = e2;
        this.dragging = Math.abs(t - this.initialMouseDownPosition.x) + Math.abs(n - this.initialMouseDownPosition.y) > Qe;
      }
      this.dragging && (this.setOverlayVisibility(false), x.dragging(this, e2));
    }, this.documentMouseUpListener = (e2) => {
      if (this.initialMouseDownPosition = null, document.removeEventListener("mousemove", this.documentDraggingMouseMoveEventListener), this.dragging) {
        const t = x.dragging(this, e2);
        vn.setActivationButtonPosition(t), this.setOverlayVisibility(true);
      } else
        this.setMenuBarOnClick();
      this.dragging = false;
    }, this.closeMenuMouseMoveListener = (e2) => {
      e2.composedPath().some((i) => {
        if (i instanceof HTMLElement) {
          const o = i;
          if (o.localName === this.localName || o.localName === "vaadin-menu-bar-overlay" && o.classList.contains("activation-button-menu"))
            return true;
        }
        return this.checkPointerIsInRangeInSurroundingRectangle(e2);
      }) || this.closeMenu();
    }, this.checkPointerIsInRangeInSurroundingRectangle = (e2) => {
      var _a2, _b2;
      const n = (_b2 = (_a2 = document.querySelector("copilot-main")) == null ? void 0 : _a2.shadowRoot) == null ? void 0 : _b2.querySelector("vaadin-menu-bar-overlay.activation-button-menu"), i = this.menubar;
      if (!n)
        return false;
      const o = n.querySelector("vaadin-menu-bar-list-box");
      if (!o)
        return false;
      const s = o.getBoundingClientRect(), r = i.getBoundingClientRect(), h2 = Math.min(s.x, r.x), u2 = Math.min(s.y, r.y), v = Math.max(s.width, r.width), y2 = s.height + r.height;
      return Qc(new DOMRect(h2, u2, v, y2), e2.clientX, e2.clientY);
    }, this.dispatchSpotlightActivationEvent = (e2) => {
      this.dispatchEvent(
        new CustomEvent("spotlight-activation-changed", {
          detail: e2
        })
      );
    }, this.activationBtnClicked = (e2) => {
      if (p$1.active && this.handleAttentionRequiredOnClick()) {
        e2 == null ? void 0 : e2.stopPropagation(), e2 == null ? void 0 : e2.preventDefault();
        return;
      }
      e2 == null ? void 0 : e2.stopPropagation(), this.dispatchEvent(new CustomEvent("activation-btn-clicked"));
    }, this.handleAttentionRequiredOnClick = () => {
      const e2 = ae.getAttentionRequiredPanelConfiguration();
      return e2 ? e2.panel && !e2.floating ? (y.emit("open-attention-required-drawer", null), true) : (ae.clearAttention(), true) : false;
    }, this.closeMenu = () => {
      this.menubar._close(), document.removeEventListener("mousemove", this.closeMenuMouseMoveListener);
    }, this.setMenuBarOnClick = () => {
      const e2 = this.shadowRoot.querySelector("vaadin-menu-bar-button");
      e2 && (e2.onclick = this.activationBtnClicked);
    };
  }
  static get styles() {
    return [
      te$1(pc),
      al`
        :host {
          --space: 8px;
          --height: 28px;
          --width: 28px;
          position: absolute;
          top: clamp(var(--space), var(--top), calc(100vh - var(--height) - var(--space)));
          left: clamp(var(--space), var(--left), calc(100vw - var(--width) - var(--space)));
          bottom: clamp(var(--space), var(--bottom), calc(100vh - var(--height) - var(--space)));
          right: clamp(var(--space), var(--right), calc(100vw - var(--width) - var(--space)));
          user-select: none;
          -ms-user-select: none;
          -moz-user-select: none;
          -webkit-user-select: none;
          --indicator-color: var(--red);
          /* Don't add a z-index or anything else that creates a stacking context */
        }

        :host .menu-button {
          min-width: unset;
        }

        :host([document-hidden]) {
          -webkit-filter: grayscale(100%); /* Chrome, Safari, Opera */
          filter: grayscale(100%);
        }

        .menu-button::part(container) {
          overflow: visible;
        }

        .menu-button vaadin-menu-bar-button {
          all: initial;
          display: block;
          position: relative;
          z-index: var(--z-index-activation-button);
          width: var(--width);
          height: var(--height);
          overflow: hidden;
          color: transparent;
          background: hsl(0 0% 0% / 0.25);
          border-radius: 8px;
          box-shadow: 0 0 0 1px hsl(0 0% 100% / 0.1);
          cursor: default;
          -webkit-backdrop-filter: blur(8px);
          backdrop-filter: blur(8px);
          transition:
            box-shadow 0.2s,
            background-color 0.2s;
        }

        /* visual effect when active */

        .menu-button vaadin-menu-bar-button::before {
          all: initial;
          content: '';
          background-image:
            radial-gradient(circle at 50% -10%, hsl(221 100% 55% / 0.6) 0%, transparent 60%),
            radial-gradient(circle at 25% 40%, hsl(303 71% 64%) 0%, transparent 70%),
            radial-gradient(circle at 80% 10%, hsla(262, 38%, 9%, 0.5) 0%, transparent 80%),
            radial-gradient(circle at 110% 50%, hsla(147, 100%, 77%, 1) 20%, transparent 100%);
          animation: 5s swirl linear infinite;
          animation-play-state: paused;
          inset: -6px;
          opacity: 0;
          position: absolute;
          transition: opacity 0.5s;
        }

        /* vaadin symbol */

        .menu-button vaadin-menu-bar-button::after {
          all: initial;
          content: '';
          position: absolute;
          inset: 1px;
          background: url('data:image/svg+xml;utf8,<svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M12.7407 9.70401C12.7407 9.74417 12.7378 9.77811 12.7335 9.81479C12.7111 10.207 12.3897 10.5195 11.9955 10.5195C11.6014 10.5195 11.2801 10.209 11.2577 9.8169C11.2534 9.7801 11.2504 9.74417 11.2504 9.70401C11.2504 9.31225 11.1572 8.90867 10.2102 8.90867H7.04307C5.61481 8.90867 5 8.22698 5 6.86345V5.70358C5 5.31505 5.29521 5 5.68008 5C6.06495 5 6.35683 5.31505 6.35683 5.70358V6.09547C6.35683 6.53423 6.655 6.85413 7.307 6.85413H10.4119C11.8248 6.85413 11.9334 7.91255 11.98 8.4729H12.0111C12.0577 7.91255 12.1663 6.85413 13.5791 6.85413H16.6841C17.3361 6.85413 17.614 6.54529 17.614 6.10641L17.6158 5.70358C17.6158 5.31505 17.9246 5 18.3095 5C18.6943 5 19 5.31505 19 5.70358V6.86345C19 8.22698 18.3763 8.90867 16.9481 8.90867H13.7809C12.8338 8.90867 12.7407 9.31225 12.7407 9.70401Z" fill="white"/><path d="M12.7536 17.7785C12.6267 18.0629 12.3469 18.2608 12.0211 18.2608C11.6907 18.2608 11.4072 18.0575 11.2831 17.7668C11.2817 17.7643 11.2803 17.7619 11.279 17.7595C11.2761 17.7544 11.2732 17.7495 11.2704 17.744L8.45986 12.4362C8.3821 12.2973 8.34106 12.1399 8.34106 11.9807C8.34106 11.4732 8.74546 11.0603 9.24238 11.0603C9.64162 11.0603 9.91294 11.2597 10.0985 11.6922L12.0216 15.3527L13.9468 11.6878C14.1301 11.2597 14.4014 11.0603 14.8008 11.0603C15.2978 11.0603 15.7021 11.4732 15.7021 11.9807C15.7021 12.1399 15.6611 12.2973 15.5826 12.4374L12.7724 17.7446C12.7683 17.7524 12.7642 17.7597 12.7601 17.767C12.7579 17.7708 12.7557 17.7746 12.7536 17.7785Z" fill="white"/></svg>');
          background-size: 100%;
        }

        .menu-button vaadin-menu-bar-button[focus-ring] {
          outline: 2px solid var(--selection-color);
          outline-offset: 2px;
        }

        .menu-button vaadin-menu-bar-button:hover {
          background: hsl(0 0% 0% / 0.8);
          box-shadow:
            0 0 0 1px hsl(0 0% 100% / 0.1),
            0 2px 8px -1px hsl(0 0% 0% / 0.3);
        }

        :host([active]) .menu-button vaadin-menu-bar-button {
          background-color: transparent;
          box-shadow:
            inset 0 0 0 1px hsl(0 0% 0% / 0.2),
            0 2px 8px -1px hsl(0 0% 0% / 0.3);
        }

        :host([active]) .menu-button vaadin-menu-bar-button::before {
          opacity: 1;
          animation-play-state: running;
        }

        :host([attention-required]) {
          animation: bounce 0.5s;
          animation-iteration-count: 2;
        }

        [part='indicator'] {
          top: -6px;
          right: -6px;
          width: 12px;
          height: 12px;
          box-sizing: border-box;
          border-radius: 100%;
          position: absolute;
          display: var(--indicator-display, none);
          background: var(--indicator-color);
          z-index: calc(var(--z-index-activation-button) + 1);
          border: 2px solid var(--indicator-border);
        }

        :host([indicator='warning']) {
          --indicator-display: block;
          --indicator-color: var(--yellow);
        }

        :host([indicator='error']) {
          --indicator-display: block;
          --indicator-color: var(--red);
        }
      `
    ];
  }
  connectedCallback() {
    super.connectedCallback(), this.reaction(
      () => ae.attentionRequiredPanelTag,
      () => {
        this.toggleAttribute(Hc, ae.attentionRequiredPanelTag !== null), this.updateIndicator();
      }
    ), this.reaction(
      () => p$1.active,
      () => {
        this.toggleAttribute("active", p$1.active);
      },
      { fireImmediately: true }
    ), this.addEventListener("mousedown", this.mouseDownListener), document.addEventListener("mouseup", this.documentMouseUpListener);
    const e2 = vn.getActivationButtonPosition();
    e2 ? (this.style.setProperty("--left", `${e2.left}px`), this.style.setProperty("--bottom", `${e2.bottom}px`), this.style.setProperty("--right", `${e2.right}px`), this.style.setProperty("--top", `${e2.top}px`)) : (this.style.setProperty("--bottom", "var(--space)"), this.style.setProperty("--right", "var(--space)")), y.on("document-activation-change", (t) => {
      this.toggleAttribute("document-hidden", !t.detail.active);
    }), this.reaction(
      () => [p$1.jdkInfo, p$1.idePluginState],
      () => {
        this.updateIndicator();
      }
    ), this.reaction(
      () => [p$1.active, p$1.idePluginState, vn.isActivationAnimation()],
      () => {
        this.generateItems();
      }
    );
  }
  disconnectedCallback() {
    super.disconnectedCallback(), this.removeEventListener("mousedown", this.mouseDownListener), document.removeEventListener("mouseup", this.documentMouseUpListener);
  }
  updateIndicator() {
    if (this.hasAttribute(Hc)) {
      this.setAttribute("indicator", "error");
      return;
    }
    const e2 = mu();
    e2.status !== "success" ? this.setAttribute("indicator", e2.status) : this.removeAttribute("indicator");
  }
  /**
   * To hide overlay while dragging
   * @param visible
   */
  setOverlayVisibility(e2) {
    const t = this.shadowRoot.querySelector("vaadin-menu-bar-button").__overlay;
    e2 ? (t == null ? void 0 : t.style.setProperty("display", "flex"), t == null ? void 0 : t.style.setProperty("visibility", "visible")) : (t == null ? void 0 : t.style.setProperty("display", "none"), t == null ? void 0 : t.style.setProperty("visibility", "invisible"));
  }
  generateItems() {
    var _a2, _b2, _c;
    const e2 = p$1.active, t = e2 && ((_a2 = p$1.userInfo) == null ? void 0 : _a2.copilotProjectCannotLeaveLocalhost) !== true, n = e2 && !!((_c = (_b2 = p$1.idePluginState) == null ? void 0 : _b2.supportedActions) == null ? void 0 : _c.find((o) => o === "undo")), i = [
      {
        text: "Vaadin Copilot",
        children: [
          { visible: e2, component: f({ component: "copilot-activation-button-user-info" }) },
          { visible: e2, component: "hr" },
          {
            component: f({ component: "copilot-activation-button-development-workflow" }),
            action: gu
          },
          {
            component: "hr",
            visible: e2
          },
          {
            visible: n,
            component: f({
              label: "Undo",
              hint: uu.undo
            }),
            action: () => {
              y.emit("undoRedo", { undo: true });
            }
          },
          {
            visible: n,
            component: f({
              label: "Redo",
              hint: uu.redo
            }),
            action: () => {
              y.emit("undoRedo", { undo: false });
            }
          },
          {
            visible: t,
            component: f({
              label: "Toggle Command Window",
              hint: uu.toggleCommandWindow,
              style: "toggle-spotlight"
            }),
            action: () => {
              p$1.setSpotlightActive(!p$1.spotlightActive);
            }
          },
          {
            component: "hr",
            visible: e2
          },
          {
            visible: e2,
            text: "Settings",
            children: [
              {
                component: f({
                  label: "Activation shortcut enabled",
                  hint: vn.isActivationShortcut() ? "✓" : void 0
                }),
                action: () => {
                  vn.setActivationShortcut(!vn.isActivationShortcut());
                }
              },
              {
                component: f({
                  label: "Show animation when activating",
                  hint: vn.isActivationAnimation() ? "✓" : void 0
                }),
                action: () => {
                  vn.setActivationAnimation(!vn.isActivationAnimation());
                }
              },
              { component: "hr" },
              {
                visible: e2,
                component: f({
                  label: "Show welcome message"
                }),
                action: () => {
                  p$1.setWelcomeActive(true), p$1.setSpotlightActive(true);
                }
              },
              {
                visible: e2,
                component: f({
                  label: "Show keyboard shortcuts"
                }),
                action: () => {
                  ae.updatePanel("copilot-shortcuts-panel", {
                    floating: true
                  });
                }
              }
            ]
          },
          { component: "hr" },
          {
            visible: e2,
            component: f({
              label: "Tell us what you think"
              // Label used also in ScreenshotsIT.java
            }),
            action: () => {
              ae.updatePanel("copilot-feedback-panel", {
                floating: true
              });
            }
          },
          {
            component: f({
              label: '<span class="deactivate">Deactivate</span><span class="activate">Activate</span> Copilot',
              hint: vn.isActivationShortcut() ? uu.toggleCopilot : void 0
            }),
            action: () => {
              this.activationBtnClicked();
            }
          }
        ]
      }
    ];
    this.items = i.filter(Ws);
  }
  render() {
    return Be$1`
      <vaadin-menu-bar
        class="menu-button"
        .items="${this.items}"
        @item-selected="${(e2) => {
      this.handleMenuItemClick(e2.detail.value);
    }}"
        ?open-on-hover=${!this.dragging}
        @mouseenter="${() => {
      document.addEventListener("mousemove", this.closeMenuMouseMoveListener);
    }}"
        overlay-class="activation-button-menu">
      </vaadin-menu-bar>
      <div part="indicator"></div>
    `;
  }
  handleMenuItemClick(e2) {
    this.closeMenu(), e2.action && e2.action();
  }
  firstUpdated() {
    this.setMenuBarOnClick(), u(this.shadowRoot);
  }
};
N([
  h$1("vaadin-menu-bar")
], T.prototype, "menubar", 2);
N([
  b()
], T.prototype, "dragging", 2);
N([
  b()
], T.prototype, "items", 2);
T = N([
  ol("copilot-activation-button")
], T);
var et = Object.defineProperty, tt = Object.getOwnPropertyDescriptor, E = (e2, t, n, i) => {
  for (var o = i > 1 ? void 0 : i ? tt(t, n) : t, s = e2.length - 1, r; s >= 0; s--)
    (r = e2[s]) && (o = (i ? r(t, n, o) : r(o)) || o);
  return i && o && et(t, n, o), o;
};
const m = "resize-dir", X = "floating-resizing-active";
let A = class extends Dl {
  constructor() {
    super(...arguments), this.panelTag = "", this.dockingItems = [
      {
        component: q({
          icon: C.layoutRight,
          label: "Dock right"
        }),
        panel: "right"
      },
      {
        component: q({
          icon: C.layoutLeft,
          label: "Dock left"
        }),
        panel: "left"
      },
      {
        component: q({
          icon: C.layoutBottom,
          label: "Dock bottom"
        }),
        panel: "bottom"
      }
    ], this.floatingResizingStarted = false, this.resizingInDrawerStarted = false, this.toggling = false, this.rectangleBeforeResizing = null, this.floatingResizeHandlerMouseMoveListener = (e2) => {
      var _a2, _b2;
      if (!((_a2 = this.panelInfo) == null ? void 0 : _a2.floating) || this.floatingResizingStarted || !((_b2 = this.panelInfo) == null ? void 0 : _b2.expanded))
        return;
      const t = this.getBoundingClientRect(), n = Math.abs(e2.clientX - t.x), i = Math.abs(t.x + t.width - e2.clientX), o = Math.abs(e2.clientY - t.y), s = Math.abs(t.y + t.height - e2.clientY), r = Number.parseInt(
        window.getComputedStyle(this).getPropertyValue("--floating-offset-resize-threshold"),
        10
      );
      let h2 = "";
      n < r ? o < r ? (h2 = "nw-resize", this.setAttribute(m, "top left")) : s < r ? (h2 = "sw-resize", this.setAttribute(m, "bottom left")) : (h2 = "col-resize", this.setAttribute(m, "left")) : i < r ? o < r ? (h2 = "ne-resize", this.setAttribute(m, "top right")) : s < r ? (h2 = "se-resize", this.setAttribute(m, "bottom right")) : (h2 = "col-resize", this.setAttribute(m, "right")) : s < r ? (h2 = "row-resize", this.setAttribute(m, "bottom")) : o < r && (h2 = "row-resize", this.setAttribute(m, "top")), h2 !== "" ? (this.rectangleBeforeResizing = this.getBoundingClientRect(), this.style.setProperty("--resize-cursor", h2)) : (this.style.removeProperty("--resize-cursor"), this.removeAttribute(m)), this.toggleAttribute(X, h2 !== "");
    }, this.floatingResizingMouseDownListener = (e2) => {
      if (!this.hasAttribute(X) || e2.button !== 0)
        return;
      e2.stopPropagation(), e2.preventDefault(), x.anchorLeftTop(this), this.floatingResizingStarted = true, this.toggleAttribute("resizing", true);
      const t = this.getResizeDirections(), { clientX: n, clientY: i } = e2;
      (t.includes("top") || t.includes("bottom")) && this.style.setProperty("--section-height", null), t.forEach((o) => this.setResizePosition(o, n, i)), p$1.setSectionPanelResizing(true);
    }, this.floatingResizingMouseLeaveListener = () => {
      var _a2;
      ((_a2 = this.panelInfo) == null ? void 0 : _a2.floating) && (this.floatingResizingStarted || (this.removeAttribute("resizing"), this.removeAttribute(X), this.removeAttribute("dragging"), this.style.removeProperty("--resize-cursor"), this.removeAttribute(m)));
    }, this.floatingResizingMouseMoveListener = (e2) => {
      var _a2;
      if (!((_a2 = this.panelInfo) == null ? void 0 : _a2.floating) || !this.floatingResizingStarted)
        return;
      e2.stopPropagation(), e2.preventDefault();
      const t = this.getResizeDirections(), { clientX: n, clientY: i } = e2;
      t.forEach((o) => this.setResizePosition(o, n, i));
    }, this.setFloatingResizeDirectionProps = (e2, t, n, i) => {
      n && n > Number.parseFloat(window.getComputedStyle(this).getPropertyValue("--min-width")) && (this.style.setProperty(`--${e2}`, `${t}px`), this.style.setProperty("width", `${n}px`));
      const o = window.getComputedStyle(this), s = Number.parseFloat(o.getPropertyValue("--header-height")), r = Number.parseFloat(o.getPropertyValue("--floating-offset-resize-threshold")) / 2;
      i && i > s + r && (this.style.setProperty(`--${e2}`, `${t}px`), this.style.setProperty("height", `${i}px`), this.container.style.setProperty("margin-top", "calc(var(--floating-offset-resize-threshold) / 4)"), this.container.style.height = `calc(${i}px - var(--floating-offset-resize-threshold) / 2)`);
    }, this.floatingResizingMouseUpListener = (e2) => {
      var _a2;
      if (!this.floatingResizingStarted || !((_a2 = this.panelInfo) == null ? void 0 : _a2.floating))
        return;
      e2.stopPropagation(), e2.preventDefault(), this.floatingResizingStarted = false, p$1.setSectionPanelResizing(false);
      const { width: t, height: n } = this.getBoundingClientRect(), { left: i, top: o, bottom: s, right: r } = x.anchor(this), h2 = window.getComputedStyle(this.container), u2 = Number.parseInt(h2.borderTopWidth, 10), v = Number.parseInt(h2.borderBottomWidth, 10);
      ae.updatePanel(this.panelInfo.tag, {
        width: t,
        height: n - (u2 + v),
        floatingPosition: {
          ...this.panelInfo.floatingPosition,
          left: i,
          top: o,
          bottom: s,
          right: r
        }
      }), this.style.removeProperty("width"), this.style.removeProperty("height"), this.container.style.removeProperty("height"), this.container.style.removeProperty("margin-top"), this.setCssSizePositionProperties(), this.toggleAttribute("dragging", false);
    }, this.transitionEndEventListener = () => {
      this.toggling && (this.toggling = false, x.anchor(this));
    }, this.resizeInDrawerMouseDownListener = (e2) => {
      e2.button === 0 && (this.resizingInDrawerStarted = true, this.setAttribute("resizing", ""), y.emit("user-select", { allowSelection: false }));
    }, this.resizeInDrawerMouseMoveListener = (e2) => {
      if (!this.resizingInDrawerStarted)
        return;
      const { y: t } = e2;
      e2.stopPropagation(), e2.preventDefault();
      const n = t - this.getBoundingClientRect().top;
      this.style.setProperty("--section-height", `${n}px`), ae.updatePanel(this.panelInfo.tag, {
        height: n
      });
    }, this.resizeInDrawerMouseUpListener = () => {
      var _a2;
      this.resizingInDrawerStarted && (((_a2 = this.panelInfo) == null ? void 0 : _a2.floating) || (this.resizingInDrawerStarted = false, this.removeAttribute("resizing"), y.emit("user-select", { allowSelection: true }), this.style.setProperty("--section-height", `${this.getBoundingClientRect().height}px`)));
    }, this.sectionPanelMouseEnterListener = () => {
      this.hasAttribute(Hc) && (this.removeAttribute(Hc), ae.clearAttention());
    }, this.contentAreaMouseDownListener = () => {
      ae.bringToFront(this.panelInfo.tag);
    }, this.documentMouseUpEventListener = () => {
      var _a2;
      document.removeEventListener("mousemove", this.draggingEventListener), ((_a2 = this.panelInfo) == null ? void 0 : _a2.floating) && (this.toggleAttribute("dragging", false), p$1.setSectionPanelDragging(false));
    }, this.panelHeaderMouseDownEventListener = (e2) => {
      e2.button === 0 && (ae.bringToFront(this.panelInfo.tag), !this.hasAttribute(m) && (e2.target instanceof HTMLButtonElement && e2.target.getAttribute("part") === "title-button" ? this.startDraggingDebounce(e2) : this.startDragging(e2)));
    }, this.panelHeaderMouseUpEventListener = (e2) => {
      e2.button === 0 && this.startDraggingDebounce.clear();
    }, this.startDragging = (e2) => {
      var _a2;
      x.draggingStarts(this, e2), document.addEventListener("mousemove", this.draggingEventListener), p$1.setSectionPanelDragging(true), ((_a2 = this.panelInfo) == null ? void 0 : _a2.floating) ? this.toggleAttribute("dragging", true) : this.parentElement.sectionPanelDraggingStarted(this, e2), e2.preventDefault(), e2.stopPropagation();
    }, this.startDraggingDebounce = Nc(this.startDragging, 200), this.draggingEventListener = (e2) => {
      var _a2, _b2;
      const t = x.dragging(this, e2);
      if (((_a2 = this.panelInfo) == null ? void 0 : _a2.floating) && ((_b2 = this.panelInfo) == null ? void 0 : _b2.floatingPosition)) {
        e2.preventDefault();
        const { left: n, top: i, bottom: o, right: s } = t;
        ae.updatePanel(this.panelInfo.tag, {
          floatingPosition: {
            ...this.panelInfo.floatingPosition,
            left: n,
            top: i,
            bottom: o,
            right: s
          }
        });
      }
    }, this.setCssSizePositionProperties = () => {
      var _a2;
      const e2 = ae.getPanelByTag(this.panelTag);
      if (e2 && (e2.height !== void 0 && (((_a2 = this.panelInfo) == null ? void 0 : _a2.floating) || e2.panel === "left" || e2.panel === "right" ? this.style.setProperty("--section-height", `${e2.height}px`) : this.style.removeProperty("--section-height")), e2.width !== void 0 && (e2.floating || e2.panel === "bottom" ? this.style.setProperty("--section-width", `${e2.width}px`) : this.style.removeProperty("--section-width")), e2.floating && e2.floatingPosition && !this.toggling)) {
        const { left: t, top: n, bottom: i, right: o } = e2.floatingPosition;
        this.style.setProperty("--left", t !== void 0 ? `${t}px` : "auto"), this.style.setProperty("--top", n !== void 0 ? `${n}px` : "auto"), this.style.setProperty("--bottom", i !== void 0 ? `${i}px` : ""), this.style.setProperty("--right", o !== void 0 ? `${o}px` : "");
      }
    }, this.renderPopupButton = () => {
      if (!this.panelInfo)
        return O$1;
      let e2;
      return this.panelInfo.panel === void 0 ? e2 = "Close the popup" : e2 = this.panelInfo.floating ? `Dock ${this.panelInfo.header} to ${this.panelInfo.panel}` : `Open ${this.panelInfo.header} as a popup`, Be$1`
      <vaadin-context-menu .items=${this.dockingItems} @item-selected="${this.changeDockingPanel}">
        <button
          @click="${(t) => this.changePanelFloating(t)}"
          @mousedown="${(t) => t.stopPropagation()}"
          aria-label=${e2}
          class="icon"
          part="popup-button"
          title="${e2}">
          ${this.getPopupButtonIcon()}
        </button>
      </vaadin-context-menu>
    `;
    }, this.changePanelFloating = (e2) => {
      var _a2, _b2, _c;
      if (this.panelInfo)
        if (e2.stopPropagation(), du(this), (_a2 = this.panelInfo) == null ? void 0 : _a2.floating)
          ae.updatePanel(this.panelInfo.tag, { floating: false });
        else {
          let t;
          if (this.panelInfo.floatingPosition)
            t = this.panelInfo.floatingPosition;
          else {
            const { left: o, top: s } = this.getBoundingClientRect();
            t = {
              left: o,
              top: s
            };
          }
          let n = (_b2 = this.panelInfo) == null ? void 0 : _b2.height;
          n === void 0 && this.panelInfo.expanded && (n = Number.parseInt(window.getComputedStyle(this).height, 10)), this.parentElement.forceClose(), ae.updatePanel(this.panelInfo.tag, {
            floating: true,
            expanded: true,
            width: ((_c = this.panelInfo) == null ? void 0 : _c.width) || Number.parseInt(window.getComputedStyle(this).width, 10),
            height: n,
            floatingPosition: t
          }), ae.bringToFront(this.panelInfo.tag);
        }
    }, this.toggleExpand = (e2) => {
      this.panelInfo && (e2.stopPropagation(), x.anchorLeftTop(this), ae.updatePanel(this.panelInfo.tag, {
        expanded: !this.panelInfo.expanded
      }), this.toggling = true, this.toggleAttribute("expanded", this.panelInfo.expanded));
    };
  }
  static get styles() {
    return [
      te$1(pc),
      te$1(mc),
      al`
        * {
          box-sizing: border-box;
        }

        :host {
          flex: none;
          display: grid;
          align-content: start;
          grid-template-rows: auto 1fr;
          transition: grid-template-rows var(--duration-2);
          overflow: hidden;
          position: relative;
          --min-width: 160px;
          --resize-div-size: 10px;
          --header-height: 36px;
          --content-height: calc(var(--section-height) - var(--header-height));
          --content-width: var(--content-width, 100%);
          --floating-border-width: 1px;
          --floating-offset-resize-threshold: 8px;
          cursor: var(--cursor, var(--resize-cursor, default));
        }

        :host(:not([expanded])) {
          grid-template-rows: auto 0fr;
          --content-height: 0px !important;
        }

        [part='header'] {
          align-items: center;
          color: var(--color-high-contrast);
          display: flex;
          flex: none;
          font: var(--font-small-medium);
          gap: var(--space-50);
          justify-content: space-between;
          min-width: 100%;
          padding: var(--space-50);
          user-select: none;
          -webkit-user-select: none;
          width: var(--min-width);
        }

        :host([floating]:not([expanded])) [part='header'] {
          --min-width: unset;
        }

        :host([floating]) [part='header'] {
          transition: border-color var(--duration-2);
        }

        :host([floating]:not([expanded])) [part='header'] {
          border-color: transparent;
        }

        [part='title'] {
          flex: auto;
          margin: 0;
          overflow: hidden;
          text-overflow: ellipsis;
        }

        [part='title']:first-child {
          margin-inline-start: var(--space-100);
        }

        [part='content'] {
          height: var(--content-height);
          overflow: auto;
          transition:
            height var(--duration-2),
            width var(--duration-2),
            opacity var(--duration-2),
            visibility calc(var(--duration-2) * 2);
        }

        [part='drawer-resize'] {
          resize: vertical;
          cursor: row-resize;
          position: absolute;
          bottom: -5px;
          left: 0;
          width: 100%;
          height: 10px;
        }

        :host([floating]) [part='drawer-resize'] {
          display: none;
        }

        :host(:not([expanded])) [part='drawer-resize'] {
          display: none;
        }

        :host(:not([floating]):not(:last-child)) {
          border-bottom: 1px solid var(--border-color);
        }

        :host(:not([expanded])) [part='content'] {
          opacity: 0;
          visibility: hidden;
        }

        :host([floating]:not([expanded])) [part='content'] {
          width: 0;
          height: 0;
        }

        :host(:not([expanded])) [part='content'][style*='height'] {
          height: 0 !important;
        }

        :host(:not([expanded])) [part='content'][style*='width'] {
          width: 0 !important;
        }

        :host([floating]) {
          position: fixed;
          min-width: 0;
          min-height: 0;
          z-index: calc(var(--z-index-floating-panel) + var(--z-index-focus, 0));
          top: clamp(0px, var(--top), calc(100vh - var(--section-height, var(--header-height)) * 0.5));
          left: clamp(calc(var(--section-width) * -0.5), var(--left), calc(100vw - var(--section-width) * 0.5));
          bottom: clamp(
            calc(var(--section-height, var(--header-height)) * -0.5),
            var(--bottom),
            calc(100vh - var(--section-height, var(--header-height)) * 0.5)
          );
          right: clamp(calc(var(--section-width) * -0.5), var(--right), calc(100vw - var(--section-width) * 0.5));
          width: var(--section-width);
          overflow: visible;
        }
        :host([floating]) [part='container'] {
          background: var(--surface);
          border: var(--floating-border-width) solid var(--surface-border-color);
          -webkit-backdrop-filter: var(--surface-backdrop-filter);
          backdrop-filter: var(--surface-backdrop-filter);
          border-radius: var(--radius-2);
          margin: auto;
          box-shadow: var(--surface-box-shadow-2);
        }
        [part='container'] {
          overflow: hidden;
        }
        :host([floating][expanded]) {
          max-height: 100vh;
        }
        :host([floating][expanded]) [part='container'] {
          height: calc(100% - var(--floating-offset-resize-threshold) / 2);
          width: calc(100% - var(--floating-offset-resize-threshold) / 2);
        }

        :host([floating]:not([expanded])) {
          width: unset;
        }

        :host([floating]) .drag-handle {
          cursor: var(--resize-cursor, move);
        }

        :host([floating][expanded]) [part='content'] {
          min-width: var(--min-width);
          min-height: 0;
          width: var(--content-width);
        }

        /* :hover for Firefox, :active for others */

        :host([floating][expanded]) [part='content']:is(:hover, :active) {
          transition: none;
        }

        [part='title-button'] {
          font-weight: var(--font-weight-bold);
          padding: 0;
          text-align: start;
        }

        [part='toggle-button'] svg {
          transition: transform 0.15s cubic-bezier(0.2, 0, 0, 1);
        }

        [part='toggle-button'][aria-expanded='true'] svg {
          transform: rotate(90deg);
        }

        .actions {
          display: none;
        }

        :host([expanded]) .actions {
          display: contents;
        }

        ::slotted(*) {
          box-sizing: border-box;
          display: block;
          height: var(--content-height, var(--default-content-height, 100%));
          /* padding: var(--space-150); */
          width: 100%;
        }

        /*workaround for outline to have a explicit height while floating by default. 
          may be removed after https://github.com/vaadin/web-components/issues/7620 is solved
        */
        :host([floating][expanded][paneltag='copilot-outline-panel']) {
          --grid-default-height: 400px;
        }

        :host([dragging]) {
          opacity: 0.4;
        }

        :host([dragging]) [part='content'] {
          pointer-events: none;
        }

        :host([resizing]),
        :host([resizing]) [part='content'] {
          transition: none;
        }
        :host([resizing]) [part='content'] {
          height: 100%;
        }

        :host([hiding-while-drag-and-drop]) {
          display: none;
        }

        // dragging in drawer

        :host(:not([floating])) .drag-handle {
          cursor: grab;
        }

        :host(:not([floating])[dragging]) .drag-handle {
          cursor: grabbing;
        }
      `
    ];
  }
  connectedCallback() {
    super.connectedCallback(), this.setAttribute("role", "region"), this.reaction(
      () => ae.getAttentionRequiredPanelConfiguration(),
      () => {
        const e2 = ae.getAttentionRequiredPanelConfiguration();
        this.toggleAttribute(Hc, (e2 == null ? void 0 : e2.tag) === this.panelTag && (e2 == null ? void 0 : e2.floating));
      }
    ), this.addEventListener("mouseenter", this.sectionPanelMouseEnterListener), document.addEventListener("mousemove", this.resizeInDrawerMouseMoveListener), document.addEventListener("mouseup", this.resizeInDrawerMouseUpListener), this.reaction(
      () => p$1.operationInProgress,
      () => {
        requestAnimationFrame(() => {
          var _a2;
          this.toggleAttribute(
            "hiding-while-drag-and-drop",
            p$1.operationInProgress === lu.DragAndDrop && ((_a2 = this.panelInfo) == null ? void 0 : _a2.floating) && !this.panelInfo.showWhileDragging
          );
        });
      }
    ), this.reaction(
      () => ae.floatingPanelsZIndexOrder,
      () => {
        this.style.setProperty("--z-index-focus", `${ae.getFloatingPanelZIndex(this.panelTag)}`);
      },
      { fireImmediately: true }
    ), this.addEventListener("transitionend", this.transitionEndEventListener), this.addEventListener("mousemove", this.floatingResizeHandlerMouseMoveListener), this.addEventListener("mousedown", this.floatingResizingMouseDownListener), this.addEventListener("mouseleave", this.floatingResizingMouseLeaveListener), document.addEventListener("mousemove", this.floatingResizingMouseMoveListener), document.addEventListener("mouseup", this.floatingResizingMouseUpListener);
  }
  disconnectedCallback() {
    super.disconnectedCallback(), this.removeEventListener("mouseenter", this.sectionPanelMouseEnterListener), this.drawerResizeElement.removeEventListener("mousedown", this.resizeInDrawerMouseDownListener), document.removeEventListener("mousemove", this.resizeInDrawerMouseMoveListener), document.removeEventListener("mouseup", this.resizeInDrawerMouseUpListener), this.removeEventListener("mousemove", this.floatingResizeHandlerMouseMoveListener), this.removeEventListener("mousedown", this.floatingResizingMouseDownListener), document.removeEventListener("mousemove", this.floatingResizingMouseMoveListener), document.removeEventListener("mouseup", this.floatingResizingMouseUpListener);
  }
  setResizePosition(e2, t, n) {
    const i = this.rectangleBeforeResizing, o = 0, s = window.innerWidth, r = 0, h2 = window.innerHeight, u2 = Math.max(o, Math.min(s, t)), v = Math.max(r, Math.min(h2, n));
    if (e2 === "left")
      this.setFloatingResizeDirectionProps(
        "left",
        u2,
        i.left - u2 + i.width
      );
    else if (e2 === "right")
      this.setFloatingResizeDirectionProps(
        "right",
        u2,
        u2 - i.right + i.width
      );
    else if (e2 === "top") {
      const y2 = i.top - v + i.height;
      this.setFloatingResizeDirectionProps("top", v, void 0, y2);
    } else if (e2 === "bottom") {
      const y2 = v - i.bottom + i.height;
      this.setFloatingResizeDirectionProps("bottom", v, void 0, y2);
    }
  }
  willUpdate(e2) {
    var _a2;
    super.willUpdate(e2), e2.has("panelTag") && (this.panelInfo = ae.getPanelByTag(this.panelTag), this.setAttribute("aria-labelledby", this.panelInfo.tag.concat("-title"))), this.toggleAttribute("floating", (_a2 = this.panelInfo) == null ? void 0 : _a2.floating);
  }
  updated(e2) {
    super.updated(e2), this.setCssSizePositionProperties();
  }
  firstUpdated(e2) {
    var _a2, _b2;
    super.firstUpdated(e2), document.addEventListener("mouseup", this.documentMouseUpEventListener), this.headerDraggableArea.addEventListener("mousedown", this.panelHeaderMouseDownEventListener), this.headerDraggableArea.addEventListener("mouseup", this.panelHeaderMouseUpEventListener), this.toggleAttribute("expanded", (_a2 = this.panelInfo) == null ? void 0 : _a2.expanded), this.toggleAttribute("individual", ((_b2 = this.panelInfo) == null ? void 0 : _b2.individual) ?? false), fu(this), this.setCssSizePositionProperties(), this.contentArea.addEventListener("mousedown", this.contentAreaMouseDownListener), this.drawerResizeElement.addEventListener("mousedown", this.resizeInDrawerMouseDownListener), u(this.shadowRoot);
  }
  render() {
    return this.panelInfo ? Be$1`
      <div part="container">
        <div part="header" class="drag-handle">
          ${this.panelInfo.expandable !== false ? Be$1` <button
                @mousedown="${(e2) => e2.stopPropagation()}"
                @click="${(e2) => this.toggleExpand(e2)}"
                aria-controls="content"
                aria-expanded="${this.panelInfo.expanded}"
                aria-label="Expand ${this.panelInfo.header}"
                class="icon"
                part="toggle-button">
                <span>${C.chevronRight}</span>
              </button>` : O$1}
          <h2 id="${this.panelInfo.tag}-title" part="title">
            <button
              part="title-button"
              @dblclick="${(e2) => {
      this.toggleExpand(e2), this.startDraggingDebounce.clear();
    }}">
              ${ae.getPanelHeader(this.panelInfo)}
            </button>
          </h2>
          <div class="actions" @mousedown="${(e2) => e2.stopPropagation()}">${this.renderActions()}</div>
          ${this.renderHelpButton()} ${this.renderPopupButton()}
        </div>
        <div part="content" id="content">
          <slot name="content"></slot>
        </div>
        <div part="drawer-resize"></div>
      </div>
    ` : O$1;
  }
  getPopupButtonIcon() {
    return this.panelInfo ? this.panelInfo.panel === void 0 ? C.x : this.panelInfo.floating ? this.panelInfo.panel === "bottom" ? C.layoutBottom : this.panelInfo.panel === "left" ? C.layoutLeft : this.panelInfo.panel === "right" ? C.layoutRight : O$1 : C.share : O$1;
  }
  renderHelpButton() {
    var _a2;
    return ((_a2 = this.panelInfo) == null ? void 0 : _a2.helpUrl) ? Be$1` <button
      @click="${() => window.open(this.panelInfo.helpUrl, "_blank")}"
      @mousedown="${(e2) => e2.stopPropagation()}"
      title="More information about ${this.panelInfo.header}"
      aria-label="More information about ${this.panelInfo.header}">
      ${C.help}
    </button>` : O$1;
  }
  renderActions() {
    var _a2;
    if (!((_a2 = this.panelInfo) == null ? void 0 : _a2.actionsTag))
      return O$1;
    const e2 = this.panelInfo.actionsTag;
    return Rl(`<${e2}></${e2}>`);
  }
  changeDockingPanel(e2) {
    var _a2;
    const t = e2.detail.value.panel;
    if (((_a2 = this.panelInfo) == null ? void 0 : _a2.panel) !== t) {
      const n = ae.panels.filter((i) => i.panel === t).map((i) => i.panelOrder).sort((i, o) => o - i)[0];
      du(this), ae.updatePanel(this.panelInfo.tag, { panel: t, panelOrder: n + 1 });
    }
    this.panelInfo.floating && this.changePanelFloating(e2);
  }
  getResizeDirections() {
    const e2 = this.getAttribute(m);
    return e2 ? e2.split(" ") : [];
  }
};
E([
  h()
], A.prototype, "panelTag", 2);
E([
  h$1(".drag-handle")
], A.prototype, "headerDraggableArea", 2);
E([
  h$1("#content")
], A.prototype, "contentArea", 2);
E([
  h$1('[part="drawer-resize"]')
], A.prototype, "drawerResizeElement", 2);
E([
  h$1('[part="container"]')
], A.prototype, "container", 2);
E([
  b()
], A.prototype, "dockingItems", 2);
A = E([
  ol("copilot-section-panel-wrapper")
], A);
function it(e2) {
  p$1.setOperationWaitsHmrUpdate(e2, 3e4);
}
y.on("undoRedo", (e2) => {
  const n = { files: e2.detail.files ?? Fc(), uiId: Gc() }, i = e2.detail.undo ? "copilot-plugin-undo" : "copilot-plugin-redo", o = e2.detail.undo ? "undo" : "redo";
  bt(o), it(lu.RedoUndo), y.send(i, n);
});
var ot = (e2, t, n, i) => {
  for (var o = t, s = e2.length - 1, r; s >= 0; s--)
    (r = e2[s]) && (o = r(o) || o);
  return o;
};
let oe = class extends Dl {
  static get styles() {
    return [
      te$1(pc),
      te$1(mc),
      te$1(yc),
      al`
        :host {
          --lumo-secondary-text-color: var(--dev-tools-text-color);
          --lumo-contrast-80pct: var(--dev-tools-text-color-emphasis);
          --lumo-contrast-60pct: var(--dev-tools-text-color-secondary);
          --lumo-font-size-m: 14px;

          position: fixed;
          bottom: 2.5rem;
          right: 0rem;
          visibility: visible; /* Always show, even if copilot is off */
          user-select: none;
          z-index: 10000;

          --dev-tools-text-color: rgba(255, 255, 255, 0.8);

          --dev-tools-text-color-secondary: rgba(255, 255, 255, 0.65);
          --dev-tools-text-color-emphasis: rgba(255, 255, 255, 0.95);
          --dev-tools-text-color-active: rgba(255, 255, 255, 1);

          --dev-tools-background-color-inactive: rgba(45, 45, 45, 0.25);
          --dev-tools-background-color-active: rgba(45, 45, 45, 0.98);
          --dev-tools-background-color-active-blurred: rgba(45, 45, 45, 0.85);

          --dev-tools-border-radius: 0.5rem;
          --dev-tools-box-shadow: 0 0 0 1px rgba(255, 255, 255, 0.05), 0 4px 12px -2px rgba(0, 0, 0, 0.4);

          --dev-tools-blue-hsl: 206, 100%, 70%;
          --dev-tools-blue-color: hsl(var(--dev-tools-blue-hsl));
          --dev-tools-green-hsl: 145, 80%, 42%;
          --dev-tools-green-color: hsl(var(--dev-tools-green-hsl));
          --dev-tools-grey-hsl: 0, 0%, 50%;
          --dev-tools-grey-color: hsl(var(--dev-tools-grey-hsl));
          --dev-tools-yellow-hsl: 38, 98%, 64%;
          --dev-tools-yellow-color: hsl(var(--dev-tools-yellow-hsl));
          --dev-tools-red-hsl: 355, 100%, 68%;
          --dev-tools-red-color: hsl(var(--dev-tools-red-hsl));

          /* Needs to be in ms, used in JavaScript as well */
          --dev-tools-transition-duration: 180ms;
        }

        .notification-tray {
          display: flex;
          flex-direction: column-reverse;
          align-items: flex-end;
          margin: 0.5rem;
          flex: none;
        }

        @supports (backdrop-filter: blur(1px)) {
          .notification-tray div.message {
            backdrop-filter: blur(8px);
          }

          .notification-tray div.message {
            background-color: var(--dev-tools-background-color-active-blurred);
          }
        }

        .notification-tray .message {
          pointer-events: auto;
          background-color: var(--dev-tools-background-color-active);
          color: var(--dev-tools-text-color);
          max-width: 40rem;
          box-sizing: border-box;
          border-radius: var(--dev-tools-border-radius);
          margin-top: 0.5rem;
          transition: var(--dev-tools-transition-duration);
          transform-origin: bottom right;
          animation: slideIn var(--dev-tools-transition-duration);
          box-shadow: var(--dev-tools-box-shadow);
          padding-top: 0.25rem;
          padding-bottom: 0.25rem;
        }

        .notification-tray .message.animate-out {
          animation: slideOut forwards var(--dev-tools-transition-duration);
        }

        .notification-tray .message .message-details {
          word-break: break-all;
        }

        .message.information {
          --dev-tools-notification-color: var(--dev-tools-blue-color);
        }

        .message.warning {
          --dev-tools-notification-color: var(--dev-tools-yellow-color);
        }

        .message.error {
          --dev-tools-notification-color: var(--dev-tools-red-color);
        }

        .message {
          display: flex;
          padding: 0.1875rem 0.75rem 0.1875rem 2rem;
          background-clip: padding-box;
        }

        .message.log {
          padding-left: 0.75rem;
        }

        .message-content {
          max-width: 100%;
          margin-right: 0.5rem;
          -webkit-user-select: text;
          -moz-user-select: text;
          user-select: text;
        }

        .message-heading {
          position: relative;
          display: flex;
          align-items: center;
          margin: 0.125rem 0;
        }

        .message .message-details {
          font-weight: 400;
          color: var(--dev-tools-text-color-secondary);
          margin: 0.25rem 0;
          display: flex;
          flex-direction: column;
        }

        .message .message-details[hidden] {
          display: none;
        }

        .message .message-details p {
          display: inline;
          margin: 0;
          margin-right: 0.375em;
          word-break: break-word;
        }

        .message .persist {
          color: var(--dev-tools-text-color-secondary);
          white-space: nowrap;
          margin: 0.375rem 0;
          display: flex;
          align-items: center;
          position: relative;
          -webkit-user-select: none;
          -moz-user-select: none;
          user-select: none;
        }

        .message .persist::before {
          content: '';
          width: 1em;
          height: 1em;
          border-radius: 0.2em;
          margin-right: 0.375em;
          background-color: rgba(255, 255, 255, 0.3);
        }

        .message .persist:hover::before {
          background-color: rgba(255, 255, 255, 0.4);
        }

        .message .persist.on::before {
          background-color: rgba(255, 255, 255, 0.9);
        }

        .message .persist.on::after {
          content: '';
          order: -1;
          position: absolute;
          width: 0.75em;
          height: 0.25em;
          border: 2px solid var(--dev-tools-background-color-active);
          border-width: 0 0 2px 2px;
          transform: translate(0.05em, -0.05em) rotate(-45deg) scale(0.8, 0.9);
        }

        .message .dismiss-message {
          font-weight: 600;
          align-self: stretch;
          display: flex;
          align-items: center;
          padding: 0 0.25rem;
          margin-left: 0.5rem;
          color: var(--dev-tools-text-color-secondary);
        }

        .message .dismiss-message:hover {
          color: var(--dev-tools-text-color);
        }

        .message.log {
          color: var(--dev-tools-text-color-secondary);
        }

        .message:not(.log) .message-heading {
          font-weight: 500;
        }

        .message.has-details .message-heading {
          color: var(--dev-tools-text-color-emphasis);
          font-weight: 600;
        }

        .message-heading::before {
          position: absolute;
          margin-left: -1.5rem;
          display: inline-block;
          text-align: center;
          font-size: 0.875em;
          font-weight: 600;
          line-height: calc(1.25em - 2px);
          width: 14px;
          height: 14px;
          box-sizing: border-box;
          border: 1px solid transparent;
          border-radius: 50%;
        }

        .message.information .message-heading::before {
          content: 'i';
          border-color: currentColor;
          color: var(--dev-tools-notification-color);
        }

        .message.warning .message-heading::before,
        .message.error .message-heading::before {
          content: '!';
          color: var(--dev-tools-background-color-active);
          background-color: var(--dev-tools-notification-color);
        }

        .ahreflike {
          font-weight: 500;
          color: var(--dev-tools-text-color-secondary);
          text-decoration: underline;
          cursor: pointer;
        }

        @keyframes slideIn {
          from {
            transform: translateX(100%);
            opacity: 0;
          }
          to {
            transform: translateX(0%);
            opacity: 1;
          }
        }

        @keyframes slideOut {
          from {
            transform: translateX(0%);
            opacity: 1;
          }
          to {
            transform: translateX(100%);
            opacity: 0;
          }
        }

        @keyframes fade-in {
          0% {
            opacity: 0;
          }
        }

        @keyframes bounce {
          0% {
            transform: scale(0.8);
          }
          50% {
            transform: scale(1.5);
            background-color: hsla(var(--dev-tools-red-hsl), 1);
          }
          100% {
            transform: scale(1);
          }
        }
      `
    ];
  }
  render() {
    return Be$1`<div class="notification-tray">
      ${p$1.notifications.map((e2) => this.renderNotification(e2))}
    </div>`;
  }
  renderNotification(e2) {
    return Be$1`
      <div
        class="message ${e2.type} ${e2.animatingOut ? "animate-out" : ""} ${e2.details || e2.link ? "has-details" : ""}"
        data-testid="message">
        <div class="message-content">
          <div class="message-heading">${e2.message}</div>
          <div class="message-details" ?hidden="${!e2.details && !e2.link}">
            ${Yc(e2.details)}
            ${e2.link ? Be$1`<a class="ahreflike" href="${e2.link}" target="_blank">Learn more</a>` : ""}
          </div>
          ${e2.dismissId ? Be$1`<div
                class="persist ${e2.dontShowAgain ? "on" : "off"}"
                @click=${() => {
      this.toggleDontShowAgain(e2);
    }}>
                ${st(e2)}
              </div>` : ""}
        </div>
        <div
          class="dismiss-message"
          @click=${(t) => {
      Oo(e2), t.stopPropagation();
    }}>
          Dismiss
        </div>
      </div>
    `;
  }
  toggleDontShowAgain(e2) {
    e2.dontShowAgain = !e2.dontShowAgain, this.requestUpdate();
  }
};
oe = ot([
  ol("copilot-notifications-container")
], oe);
function st(e2) {
  return e2.dontShowAgainMessage ? e2.dontShowAgainMessage : "Do not show this again";
}
So({
  type: Re.WARNING,
  message: "Development Mode",
  details: "This application is running in development mode.",
  dismissId: "devmode"
});
const K = Nc(async () => {
  await jc();
});
y.on("vite-after-update", () => {
  K();
});
const se = (_b = (_a = window == null ? void 0 : window.Vaadin) == null ? void 0 : _a.connectionState) == null ? void 0 : _b.stateChangeListeners;
se ? se.add((e2, t) => {
  e2 === "loading" && t === "connected" && p$1.active && K();
}) : console.warn("Unable to add listener for connection state changes");
y.on("copilot-plugin-state", (e2) => {
  p$1.setIdePluginState(e2.detail), e2.preventDefault();
});
y.on("copilot-early-project-state", (e$1) => {
  e.setSpringSecurityEnabled(e$1.detail.springSecurityEnabled), e.setSpringJpaDataEnabled(e$1.detail.springJpaDataEnabled), e.setSupportsHilla(e$1.detail.supportsHilla), e.setUrlPrefix(e$1.detail.urlPrefix), e$1.preventDefault();
});
y.on("location-changed", (e2) => {
  K();
});
y.on("copilot-ide-notification", (e2) => {
  So({
    type: Re[e2.detail.type],
    message: e2.detail.message,
    dismissId: e2.detail.dismissId
  }), e2.preventDefault();
});
