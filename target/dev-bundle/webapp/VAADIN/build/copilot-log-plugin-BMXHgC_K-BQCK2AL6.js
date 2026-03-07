import { D as Dl, y as y$1, B as Be, p as p$1, h as ho, j as jn, Y as Yc, R as Re, K as Ks, b as bt, N as Nt, a as Bc, c as ae, o as ol } from "./indexhtml-Ckkek0fI.js";
import { b as b$1 } from "./state-DDML0umE-B6VJ4oDN.js";
import { i as i$1 } from "./base-panel-C9ezFash-BDNmZs8y.js";
import { C as C$1 } from "./icons-x5khyqlC-CIK0PyAw.js";
const B = 'copilot-log-panel ul{list-style-type:none;margin:0;padding:0}copilot-log-panel ul li{align-items:start;display:flex;gap:var(--space-50);padding:var(--space-100) var(--space-50);position:relative}copilot-log-panel ul li:before{border-bottom:1px dashed var(--divider-color);content:"";inset:auto 0 0 calc(var(--size-m) + var(--space-100));position:absolute}copilot-log-panel ul li span.icon{display:flex;flex-shrink:0;justify-content:center;width:var(--size-m)}copilot-log-panel ul li.information span.icon{color:var(--blue-color)}copilot-log-panel ul li.warning span.icon{color:var(--warning-color)}copilot-log-panel ul li.error span.icon{color:var(--error-color)}copilot-log-panel ul li .message{display:flex;flex-direction:column;flex-grow:1;overflow:hidden}copilot-log-panel ul li:not(.expanded) span{overflow:hidden;text-overflow:ellipsis;white-space:nowrap}copilot-log-panel ul li button svg{transition:transform .15s cubic-bezier(.2,0,0,1)}copilot-log-panel ul li button[aria-expanded=true] svg{transform:rotate(90deg)}copilot-log-panel ul li code{margin-top:var(--space-50)}copilot-log-panel ul li.expanded .secondary{margin-top:var(--space-100)}';
var C = Object.defineProperty, _ = Object.getOwnPropertyDescriptor, u = (e, t, a, i) => {
  for (var o = i > 1 ? void 0 : i ? _(t, a) : t, d = e.length - 1, s; d >= 0; d--)
    (s = e[d]) && (o = (i ? s(t, a, o) : s(o)) || o);
  return i && o && C(t, a, o), o;
};
class b {
  constructor() {
    this.showTimestamps = false, Nt(this);
  }
  toggleShowTimestamps() {
    this.showTimestamps = !this.showTimestamps;
  }
}
const h = new b();
let r = class extends i$1 {
  constructor() {
    super(...arguments), this.unreadErrors = false, this.messages = [], this.nextMessageId = 1, this.transitionDuration = 0, this.errorHandlersAdded = false;
  }
  connectedCallback() {
    if (super.connectedCallback(), this.onCommand("log", (e) => {
      this.handleLogEventData({ type: e.data.type, message: e.data.message });
    }), this.onEventBus("log", (e) => this.handleLogEvent(e)), this.onEventBus("update-log", (e) => this.updateLog(e.detail)), this.onEventBus("notification-shown", (e) => this.handleNotification(e)), this.onEventBus("clear-log", () => this.clear()), this.reaction(
      () => p$1.sectionPanelResizing,
      () => {
        this.requestUpdate();
      }
    ), this.transitionDuration = parseInt(
      window.getComputedStyle(this).getPropertyValue("--dev-tools-transition-duration"),
      10
    ), !this.errorHandlersAdded) {
      const e = (t) => {
        Bc(() => {
          ae.attentionRequiredPanelTag = "copilot-log-panel";
        }), this.log(Re.ERROR, t.message, !!t.internal, t.details, t.link);
      };
      ho((t) => {
        e(t);
      }), jn.forEach((t) => {
        e(t);
      }), jn.length = 0, this.errorHandlersAdded = true;
    }
  }
  clear() {
    this.messages = [];
  }
  handleNotification(e) {
    this.log(e.detail.type, e.detail.message, true, e.detail.details, e.detail.link);
  }
  handleLogEvent(e) {
    this.handleLogEventData(e.detail);
  }
  handleLogEventData(e) {
    this.log(
      e.type,
      e.message,
      !!e.internal,
      e.details,
      e.link,
      Yc(e.expandedMessage),
      Yc(e.expandedDetails),
      e.id
    );
  }
  activate() {
    this.unreadErrors = false, this.updateComplete.then(() => {
      const e = this.renderRoot.querySelector(".message:last-child");
      e && e.scrollIntoView();
    });
  }
  render() {
    return Be`
      <style>
        ${B}
      </style>
      <ul>
        ${this.messages.map((e) => this.renderMessage(e))}
      </ul>
    `;
  }
  renderMessage(e) {
    let t, a;
    return e.type === Re.ERROR ? (a = C$1.alertTriangle, t = "Error") : e.type === Re.WARNING ? (a = C$1.warning, t = "Warning") : (a = C$1.info, t = "Info"), Be`
      <li
        class="${e.type} ${e.expanded ? "expanded" : ""} ${e.details || e.link ? "has-details" : ""}"
        data-id="${e.id}">
        <span aria-label="${t}" class="icon" title="${t}">${a}</span>
        <span class="message" @click=${() => this.toggleExpanded(e)}>
          <span class="timestamp" ?hidden=${!h.showTimestamps}>${W(e.timestamp)}</span>
          <span class="primary">
            ${e.expanded && e.expandedMessage ? e.expandedMessage : e.message}
          </span>
          ${e.expanded ? Be` <span class="secondary"> ${e.expandedDetails ?? e.details} </span>` : Be` <span class="secondary" ?hidden="${!e.details && !e.link}">
                ${Yc(e.details)}
                ${e.link ? Be` <a href="${e.link}" target="_blank">Learn more</a>` : ""}
              </span>`}
        </span>
        <!-- TODO: a11y, button needs aria-controls with unique ids -->
        <button
          aria-controls="content"
          aria-expanded="${e.expanded}"
          aria-label="Expand details"
          class="icon"
          @click=${() => this.toggleExpanded(e)}
          ?hidden=${!this.canBeExpanded(e)}>
          <span>${C$1.chevronRight}</span>
        </button>
      </li>
    `;
  }
  log(e, t, a, i, o, d, s, E) {
    const T = this.nextMessageId;
    this.nextMessageId += 1, s || (s = t);
    const f = {
      id: T,
      type: e,
      message: t,
      details: i,
      link: o,
      dontShowAgain: false,
      deleted: false,
      expanded: false,
      expandedMessage: d,
      expandedDetails: s,
      timestamp: /* @__PURE__ */ new Date(),
      internal: a,
      userId: E
    };
    for (this.messages.push(f); this.messages.length > r.MAX_LOG_ROWS; )
      this.messages.shift();
    return this.requestUpdate(), this.updateComplete.then(() => {
      const m = this.renderRoot.querySelector(".message:last-child");
      m ? (setTimeout(() => m.scrollIntoView({ behavior: "smooth" }), this.transitionDuration), this.unreadErrors = false) : e === Re.ERROR && (this.unreadErrors = true);
    }), f;
  }
  updateLog(e) {
    let t = this.messages.find((a) => a.userId === e.id);
    t || (t = this.log(Re.INFORMATION, "<Log message to update was not found>", false)), Object.assign(t, e), Ks(t.expandedDetails) && (t.expandedDetails = Yc(t.expandedDetails)), this.requestUpdate();
  }
  updated() {
    var _a;
    const e = this.querySelector(".row:last-child");
    e && this.isTooLong(e.querySelector(".firstrowmessage")) && ((_a = e.querySelector("button.expand")) == null ? void 0 : _a.removeAttribute("hidden"));
  }
  toggleExpanded(e) {
    this.canBeExpanded(e) && (e.expanded = !e.expanded, this.requestUpdate()), bt("use-log", { source: "toggleExpanded" });
  }
  canBeExpanded(e) {
    var _a;
    if (e.expandedMessage || e.expanded)
      return true;
    const t = (_a = this.querySelector(`[data\\-id="${e.id}"]`)) == null ? void 0 : _a.querySelector(
      ".firstrowmessage"
    );
    return this.isTooLong(t);
  }
  isTooLong(e) {
    return e && e.offsetWidth < e.scrollWidth;
  }
};
r.MAX_LOG_ROWS = 1e3;
u([
  b$1()
], r.prototype, "unreadErrors", 2);
u([
  b$1()
], r.prototype, "messages", 2);
r = u([
  ol("copilot-log-panel")
], r);
let w = class extends Dl {
  createRenderRoot() {
    return this;
  }
  render() {
    return Be`
      <style>
        copilot-log-panel-actions {
          display: contents;
        }
      </style>
      <button
        aria-label="Clear log"
        class="icon"
        title="Clear log"
        @click=${() => {
      y$1.emit("clear-log", {});
    }}>
        <span>${C$1.trash}</span>
      </button>
      <button
        aria-label="Toggle timestamps"
        class="icon"
        title="Toggle timestamps"
        @click=${() => {
      h.toggleShowTimestamps();
    }}>
        <span class="${h.showTimestamps ? "on" : "off"}"> ${C$1.clock} </span>
      </button>
    `;
  }
};
w = u([
  ol("copilot-log-panel-actions")
], w);
const U = {
  header: "Log",
  expanded: true,
  panelOrder: 0,
  panel: "bottom",
  floating: false,
  tag: "copilot-log-panel",
  actionsTag: "copilot-log-panel-actions"
}, N = {
  init(e) {
    e.addPanel(U);
  }
};
window.Vaadin.copilot.plugins.push(N);
const y = { hour: "numeric", minute: "numeric", second: "numeric", fractionalSecondDigits: 3 };
let g;
try {
  g = new Intl.DateTimeFormat(navigator.language, y);
} catch (e) {
  console.error("Failed to create date time formatter for ", navigator.language, e), g = new Intl.DateTimeFormat("en-US", y);
}
function W(e) {
  return g.format(e);
}
export {
  w as Actions,
  r as CopilotLogPanel
};
