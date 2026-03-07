import { p as p$1, B as Be, b as bt, S as So, R as Re, o as ol } from "./indexhtml-Ckkek0fI.js";
import { i as i$1 } from "./base-panel-C9ezFash-BDNmZs8y.js";
import { C } from "./icons-x5khyqlC-CIK0PyAw.js";
const v = "copilot-features-panel{padding:var(--space-100);font:var(--font-xsmall);display:grid;grid-template-columns:auto 1fr;gap:var(--space-50);height:auto}copilot-features-panel a{display:flex;align-items:center;gap:var(--space-50);white-space:nowrap}copilot-features-panel a svg{height:12px;width:12px;min-height:12px;min-width:12px}";
var w = (e, t, a, n) => {
  for (var o = t, s = e.length - 1, i; s >= 0; s--)
    (i = e[s]) && (o = i(o) || o);
  return o;
};
const l = window.Vaadin.devTools;
let p = class extends i$1 {
  render() {
    return Be` <style>
        ${v}
      </style>
      ${p$1.featureFlags.map(
      (e) => Be`
          <copilot-toggle-button
            .title="${e.title}"
            ?checked=${e.enabled}
            @on-change=${(t) => this.toggleFeatureFlag(t, e)}>
          </copilot-toggle-button>
          <a class="ahreflike" href="${e.moreInfoLink}" title="Learn more" target="_blank"
            >learn more ${C.share}</a
          >
        `
    )}`;
  }
  toggleFeatureFlag(e, t) {
    const a = e.target.checked;
    bt("use-feature", { source: "toggle", enabled: a, id: t.id }), l.frontendConnection ? (l.frontendConnection.send("setFeature", { featureId: t.id, enabled: a }), So({
      type: Re.INFORMATION,
      message: `“${t.title}” ${a ? "enabled" : "disabled"}`,
      details: t.requiresServerRestart ? "This feature requires a server restart" : void 0,
      dismissId: `feature${t.id}${a ? "Enabled" : "Disabled"}`
    })) : l.log("error", `Unable to toggle feature ${t.title}: No server connection available`);
  }
};
p = w([
  ol("copilot-features-panel")
], p);
const $ = {
  header: "Features",
  expanded: false,
  panelOrder: 35,
  panel: "right",
  floating: false,
  tag: "copilot-features-panel",
  helpUrl: "https://vaadin.com/docs/latest/flow/configuration/feature-flags"
}, x = {
  init(e) {
    e.addPanel($);
  }
};
window.Vaadin.copilot.plugins.push(x);
export {
  p as CopilotFeaturesPanel
};
