import { D as Dl, y as y$1, O as Oc, B as Be, d as cu, E as Eo, b as bt, e as bu, p as p$1, c as ae, f as O, $ as $c, m as mu, g as gu, S as So, R as Re, o as ol } from "./indexhtml-Ckkek0fI.js";
import { b } from "./state-DDML0umE-B6VJ4oDN.js";
import { i as i$1 } from "./base-panel-C9ezFash-BDNmZs8y.js";
import { C } from "./icons-x5khyqlC-CIK0PyAw.js";
import { e } from "./early-project-state-CqEloDes-CqEloDes.js";
const W = "copilot-info-panel{--dev-tools-red-color: red;--dev-tools-grey-color: gray;--dev-tools-green-color: green;position:relative}copilot-info-panel div.info-tray{display:flex;flex-direction:column;gap:10px}copilot-info-panel button.upgrade-btn{color:var(--blue-600);position:relative;height:unset}copilot-info-panel button.upgrade-btn .new-version-indicator{--indicator-size: 6px;top:0;right:calc(-1 * var(--indicator-size) / 2);width:var(--indicator-size);height:var(--indicator-size);box-sizing:border-box;border-radius:100%;position:absolute;background:var(--accent-color);animation:ping 2s cubic-bezier(0,0,.2,1) infinite}copilot-info-panel vaadin-button{margin-inline:var(--lumo-space-l)}copilot-info-panel dl{display:grid;grid-template-columns:auto auto;gap:0;margin:var(--space-100) var(--space-50);font:var(--font-xsmall)}copilot-info-panel dl>dt,copilot-info-panel dl>dd{padding:3px 10px;margin:0;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}copilot-info-panel dd.live-reload-status>span{overflow:hidden;text-overflow:ellipsis;display:block;color:var(--status-color)}copilot-info-panel dd span.hidden{display:none}copilot-info-panel dd span.true{color:var(--dev-tools-green-color);font-size:large}copilot-info-panel dd span.false{color:var(--dev-tools-red-color);font-size:large}copilot-info-panel code{white-space:nowrap;-webkit-user-select:all;user-select:all}copilot-info-panel .checks{display:inline-grid;grid-template-columns:auto 1fr;gap:var(--space-50)}copilot-info-panel span.hint{font-size:var(--font-size-0);background:var(--gray-50);padding:var(--space-75);border-radius:var(--radius-2)}";
var k, E;
function _() {
  return E || (E = 1, k = function() {
    var e2 = document.getSelection();
    if (!e2.rangeCount)
      return function() {
      };
    for (var t = document.activeElement, o = [], l = 0; l < e2.rangeCount; l++)
      o.push(e2.getRangeAt(l));
    switch (t.tagName.toUpperCase()) {
      // .toUpperCase handles XHTML
      case "INPUT":
      case "TEXTAREA":
        t.blur();
        break;
      default:
        t = null;
        break;
    }
    return e2.removeAllRanges(), function() {
      e2.type === "Caret" && e2.removeAllRanges(), e2.rangeCount || o.forEach(function(n) {
        e2.addRange(n);
      }), t && t.focus();
    };
  }), k;
}
var D, $;
function G() {
  if ($) return D;
  $ = 1;
  var e2 = _(), t = {
    "text/plain": "Text",
    "text/html": "Url",
    default: "Text"
  }, o = "Copy to clipboard: #{key}, Enter";
  function l(r) {
    var a = (/mac os x/i.test(navigator.userAgent) ? "⌘" : "Ctrl") + "+C";
    return r.replace(/#{\s*key\s*}/g, a);
  }
  function n(r, a) {
    var s, g, f, v, p, i, w = false;
    a || (a = {}), s = a.debug || false;
    try {
      f = e2(), v = document.createRange(), p = document.getSelection(), i = document.createElement("span"), i.textContent = r, i.ariaHidden = "true", i.style.all = "unset", i.style.position = "fixed", i.style.top = 0, i.style.clip = "rect(0, 0, 0, 0)", i.style.whiteSpace = "pre", i.style.webkitUserSelect = "text", i.style.MozUserSelect = "text", i.style.msUserSelect = "text", i.style.userSelect = "text", i.addEventListener("copy", function(d) {
        if (d.stopPropagation(), a.format)
          if (d.preventDefault(), typeof d.clipboardData > "u") {
            s && console.warn("unable to use e.clipboardData"), s && console.warn("trying IE specific stuff"), window.clipboardData.clearData();
            var x = t[a.format] || t.default;
            window.clipboardData.setData(x, r);
          } else
            d.clipboardData.clearData(), d.clipboardData.setData(a.format, r);
        a.onCopy && (d.preventDefault(), a.onCopy(d.clipboardData));
      }), document.body.appendChild(i), v.selectNodeContents(i), p.addRange(v);
      var R = document.execCommand("copy");
      if (!R)
        throw new Error("copy command was unsuccessful");
      w = true;
    } catch (d) {
      s && console.error("unable to copy using execCommand: ", d), s && console.warn("trying IE specific stuff");
      try {
        window.clipboardData.setData(a.format || "text", r), a.onCopy && a.onCopy(window.clipboardData), w = true;
      } catch (x) {
        s && console.error("unable to copy using clipboardData: ", x), s && console.error("falling back to prompt"), g = l("message" in a ? a.message : o), window.prompt(g, r);
      }
    } finally {
      p && (typeof p.removeRange == "function" ? p.removeRange(v) : p.removeAllRanges()), i && document.body.removeChild(i), f();
    }
    return w;
  }
  return D = n, D;
}
var K = G();
const X = /* @__PURE__ */ Oc(K);
var Z = Object.defineProperty, Q = Object.getOwnPropertyDescriptor, h = (e2, t, o, l) => {
  for (var n = l > 1 ? void 0 : l ? Q(t, o) : t, r = e2.length - 1, a; r >= 0; r--)
    (a = e2[r]) && (n = (l ? a(t, o, n) : a(n)) || n);
  return l && n && Z(t, o, n), n;
};
let y = class extends i$1 {
  constructor() {
    super(...arguments), this.serverInfo = [], this.clientInfo = [{ name: "Browser", version: navigator.userAgent }], this.handleServerInfoEvent = (e2) => {
      const t = JSON.parse(e2.data.info);
      this.serverInfo = t.versions, cu().then((o) => {
        o && (this.clientInfo.unshift({ name: "Vaadin Employee", version: "true", more: void 0 }), this.requestUpdate("clientInfo"));
      }), Eo() === "success" && bt("hotswap-active", { value: bu() });
    };
  }
  connectedCallback() {
    super.connectedCallback(), this.onCommand("copilot-info", this.handleServerInfoEvent), this.onEventBus("system-info-with-callback", (e2) => {
      e2.detail.callback(this.getInfoForClipboard(e2.detail.notify));
    }), this.reaction(
      () => p$1.idePluginState,
      () => {
        this.requestUpdate("serverInfo");
      }
    );
  }
  getIndex(e2) {
    return this.serverInfo.findIndex((t) => t.name === e2);
  }
  render() {
    var _a;
    const e$1 = ((_a = p$1.newVaadinVersionState) == null ? void 0 : _a.versions) !== void 0 && p$1.newVaadinVersionState.versions.length > 0, t = [...this.serverInfo, ...this.clientInfo];
    let o = this.getIndex("Spring") + 1;
    o === 0 && (o = t.length), e.springSecurityEnabled && (t.splice(o, 0, { name: "Spring Security", version: "true" }), o++), e.springJpaDataEnabled && (t.splice(o, 0, { name: "Spring Data JPA", version: "true" }), o++);
    const l = t.find((n) => n.name === "Vaadin");
    return l && (l.more = Be`<button
        aria-label="Upgrade vaadin version"
        class="upgrade-btn"
        id="new-vaadin-version-btn"
        @click="${(n) => {
      n.stopPropagation(), ae.updatePanel("copilot-vaadin-versions", { floating: true });
    }}">
        Upgrade
        <span class="${e$1 ? "new-version-indicator" : ""}"></span>
      </button>`), Be` <style>
        ${W}
      </style>
      <div class="info-tray">
        <dl>
          ${t.map(
      (n) => Be`
              <dt>${n.name}</dt>
              <dd title="${n.version}" style="${n.name === "Java Hotswap" ? "white-space: normal" : ""}">
                ${this.renderValue(n.version)}
                <span class="more">${n.more}</span>
              </dd>
            `
    )}
          ${this.renderDevWorkflowSection()}
        </dl>
        ${this.renderDevelopmentWorkflowButton()}
      </div>`;
  }
  renderDevWorkflowSection() {
    const e2 = Eo(), t = this.getIdePluginLabelText(p$1.idePluginState), o = this.getHotswapAgentLabelText(e2);
    return Be`
      <dt>Java Hotswap</dt>
      <dd>${m(e2 === "success")} ${o}</dd>
      ${$c() !== "unsupported" ? Be`<dt>IDE Plugin</dt>
            <dd>${m($c() === "success")} ${t}</dd>` : O}
    `;
  }
  renderDevelopmentWorkflowButton() {
    const e2 = mu();
    let t = "", o = null;
    return e2.status === "success" ? (t = "More details...", o = C.checkCircle) : e2.status === "warning" ? (t = "Improve Development Workflow...", o = C.alertTriangle) : e2.status === "error" && (t = "Fix Development Workflow...", o = Be`<span style="color: var(--lumo-error-color)">${C.alertCircle}</span>`), Be`
      <vaadin-button
        id="development-workflow-guide"
        @click="${() => {
      gu();
    }}">
        <span slot="prefix"> ${o}</span>
        ${t}</vaadin-button
      >
    `;
  }
  getHotswapAgentLabelText(e2) {
    return e2 === "success" ? "Java Hotswap is enabled" : e2 === "error" ? "Hotswap is partially enabled" : "Hotswap is not enabled";
  }
  getIdePluginLabelText(e2) {
    if ($c() !== "success")
      return "Not installed";
    if (e2 == null ? void 0 : e2.version) {
      let t = null;
      return (e2 == null ? void 0 : e2.ide) && ((e2 == null ? void 0 : e2.ide) === "intellij" ? t = "IntelliJ" : (e2 == null ? void 0 : e2.ide) === "vscode" ? t = "VS Code" : (e2 == null ? void 0 : e2.ide) === "eclipse" && (t = "Eclipse")), t ? `${e2 == null ? void 0 : e2.version} ${t}` : e2 == null ? void 0 : e2.version;
    }
    return "Not installed";
  }
  renderValue(e2) {
    return e2 === "false" ? m(false) : e2 === "true" ? m(true) : e2;
  }
  getInfoForClipboard(e2) {
    const t = this.renderRoot.querySelectorAll(".info-tray dt"), n = Array.from(t).map((r) => ({
      key: r.textContent.trim(),
      value: r.nextElementSibling.textContent.trim()
    })).filter((r) => r.key !== "Live reload").filter((r) => !r.key.startsWith("Vaadin Emplo")).map((r) => {
      var _a;
      const { key: a } = r;
      let { value: s } = r;
      if (a === "IDE Plugin")
        s = this.getIdePluginLabelText(p$1.idePluginState) ?? "false";
      else if (a === "Java Hotswap") {
        const g = (_a = p$1.jdkInfo) == null ? void 0 : _a.jrebel, f = Eo();
        g && f === "success" ? s = "JRebel is in use" : s = this.getHotswapAgentLabelText(f);
      } else a === "Vaadin" && s.indexOf(`
`) !== -1 && (s = s.substring(0, s.indexOf(`
`)));
      return `${a}: ${s}`;
    }).join(`
`);
    return e2 && So({
      type: Re.INFORMATION,
      message: "Environment information copied to clipboard",
      dismissId: "versionInfoCopied"
    }), n.trim();
  }
};
h([
  b()
], y.prototype, "serverInfo", 2);
h([
  b()
], y.prototype, "clientInfo", 2);
y = h([
  ol("copilot-info-panel")
], y);
let T = class extends Dl {
  createRenderRoot() {
    return this;
  }
  connectedCallback() {
    super.connectedCallback(), this.style.display = "flex";
  }
  render() {
    return Be` <button title="Copy to clipboard" aria-label="Copy to clipboard" theme="icon tertiary">
      <span
        @click=${() => {
      y$1.emit("system-info-with-callback", {
        callback: X,
        notify: true
      });
    }}
        >${C.copy}</span
      >
    </button>`;
  }
};
T = h([
  ol("copilot-info-actions")
], T);
const Y = {
  header: "Info",
  expanded: false,
  panelOrder: 15,
  panel: "right",
  floating: false,
  tag: "copilot-info-panel",
  actionsTag: "copilot-info-actions",
  eager: true
  // Render even when collapsed as error handling depends on this
}, ee = {
  init(e2) {
    e2.addPanel(Y);
  }
};
window.Vaadin.copilot.plugins.push(ee);
function m(e2) {
  return e2 ? Be`<span class="true">☑</span>` : Be`<span class="false">☒</span>`;
}
export {
  T as Actions,
  y as CopilotInfoPanel
};
