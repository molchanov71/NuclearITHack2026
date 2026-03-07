import { B as Be, y, c as ae, b as bt, v as ve, i as he, s as su, o as ol } from "./indexhtml-Ckkek0fI.js";
import { b } from "./state-DDML0umE-B6VJ4oDN.js";
import { u as u$1, h } from "./overlay-monkeypatch-CfGi1TGf-B6eZkgQ0.js";
import { i as i$1 } from "./base-panel-C9ezFash-BDNmZs8y.js";
import { C } from "./icons-x5khyqlC-CIK0PyAw.js";
const $ = "copilot-feedback-panel{display:flex;flex-direction:column;font:var(--font-xsmall);--vaadin-input-field-label-font-size: var(--font-size-1);padding:var(--space-200);gap:var(--space-200);justify-content:space-between}copilot-feedback-panel>p{margin:0}copilot-feedback-panel .dialog-footer{display:flex;gap:var(--space-100)}copilot-feedback-panel vaadin-select,copilot-feedback-panel vaadin-text-area,copilot-feedback-panel vaadin-text-field{padding-top:0;--lumo-text-field-size: 1.75rem;--vaadin-input-field-label-font-size: var(--font-size-2);--vaadin-input-field-background: none;--vaadin-input-field-border-color: transparent;--vaadin-input-field-border-width: 1px;--vaadin-input-field-border-color: var(--border-color-high-contrast);--vaadin-input-field-hover-highlight: var(--gray-100);--vaadin-input-field-hover-highlight-opacity: 1}copilot-feedback-panel vaadin-text-area>textarea{max-height:7em}copilot-feedback-panel vaadin-text-area>textarea{padding:var(--space-100) 0;font:var(--font-xsmall)}copilot-feedback-panel vaadin-text-area:hover::part(input-field){background-color:var(--gray-100)}copilot-feedback-panel vaadin-text-field>input{font:var(--font-xsmall)}copilot-feedback-panel vaadin-select::part(input-field){border-radius:var(--radius-1);flex:1;padding:0 var(--space-50)}copilot-feedback-panel vaadin-select[focus-ring]::part(input-field){box-shadow:none;outline:2px solid var(--selection-color);outline-offset:-2px}copilot-feedback-panel vaadin-select-value-button{padding:0 var(--space-50)}copilot-feedback-panel vaadin-select-item{--_lumo-selected-item-height: 1.75rem;--_lumo-selected-item-padding: 0;font:var(--font-xsmall)}copilot-feedback-panel vaadin-select-item:not([disabled]):hover{background:transparent;color:var(--color-high-contrast)}copilot-feedback-panel vaadin-combo-box-item:not([disabled]):hover{background:transparent;color:var(--color-high-contrast)}";
var A = Object.defineProperty, P = Object.getOwnPropertyDescriptor, o = (e, t, l, n) => {
  for (var a = n > 1 ? void 0 : n ? P(t, l) : t, s = e.length - 1, r; s >= 0; s--)
    (r = e[s]) && (a = (n ? r(t, l, a) : r(a)) || a);
  return n && a && A(t, l, a), a;
};
const u = "https://github.com/vaadin/copilot/issues/new", T = "?template=feature_request.md&title=%5BFEATURE%5D", F = "A short, concise description of the bug and why you consider it a bug. Any details like exceptions and logs can be helpful as well.", D = "Please provide as many details as possible, this will help us deliver a fix as soon as possible.%0AThank you!%0A%0A%23%23%23 Description of the Bug%0A%0A{description}%0A%0A%23%23%23 Expected Behavior%0A%0AA description of what you would expect to happen. (Sometimes it is clear what the expected outcome is if something does not work, other times, it is not super clear.)%0A%0A%23%23%23 Minimal Reproducible Example%0A%0AWe would appreciate the minimum code with which we can reproduce the issue.%0A%0A%23%23%23 Versions%0A{versionsInfo}";
let i = class extends i$1 {
  constructor() {
    super(), this.description = "", this.items = [
      {
        label: "Report a Bug",
        value: "bug",
        ghTitle: "[BUG]"
      },
      {
        label: "Ask a Question",
        value: "question",
        ghTitle: "[QUESTION]"
      },
      {
        label: "Share an Idea",
        value: "idea",
        ghTitle: "[FEATURE]"
      }
    ];
  }
  render() {
    return Be`<style>
        ${$}</style
      >${this.renderContent()}${this.renderFooter()}`;
  }
  firstUpdated() {
    u$1(this);
  }
  renderContent() {
    return this.message === void 0 ? Be`
          <p>
            Your insights are incredibly valuable to us. Whether you’ve encountered a hiccup, have questions, or ideas
            to make our platform better, we're all ears! If you wish, leave your email and we’ll get back to you. You
            can even share your code snippet with us for a clearer picture.
          </p>
          <vaadin-select
            label="What's on your mind?"
            theme="feedback"
            .items="${this.items}"
            .value="${this.items[0].value}"
            @value-changed=${(e) => {
      this.type = e.detail.value;
    }}>
          </vaadin-select>
          <vaadin-text-area
            .value="${this.description}"
            @keydown=${this.keyDown}
            @focus=${() => {
      this.descriptionField.invalid = false, this.descriptionField.placeholder = "";
    }}
            @value-changed=${(e) => {
      this.description = e.detail.value;
    }}
            label="Tell Us More"
            helper-text="Describe what you're experiencing, wondering about, or envisioning. The more you share, the better we can understand and act on your feedback"></vaadin-text-area>
          <vaadin-text-field
            @keydown=${this.keyDown}
            @value-changed=${(e) => {
      this.email = e.detail.value;
    }}
            id="email"
            label="Your Email (Optional)"
            helper-text="Leave your email if you’d like us to follow up. Totally optional, but we’d love to keep the conversation going."></vaadin-text-field>
        ` : Be`<p>${this.message}</p>`;
  }
  renderFooter() {
    return this.message === void 0 ? Be`
          <div class="dialog-footer">
            <vaadin-button
              theme="tertiary"
              @click="${() => y.emit("system-info-with-callback", {
      callback: (e) => this.openGithub(e, this),
      notify: false
    })}">
              <span style="display: flex" slot="prefix">${C.github}</span>
              Create GitHub issue
            </vaadin-button>
            <div style="flex-grow: 1"></div>
            <vaadin-button theme="tertiary" @click="${this.close}">Cancel</vaadin-button>
            <vaadin-button theme="primary" @click="${this.submit}">Submit</vaadin-button>
          </div>
        ` : Be` <div class="footer">
          <vaadin-button @click="${this.close}">Close</vaadin-button>
        </div>`;
  }
  close() {
    ae.updatePanel("copilot-feedback-panel", {
      floating: false
    });
  }
  submit() {
    var _a;
    if (bt("feedback"), this.description.trim() === "") {
      this.descriptionField.invalid = true, this.descriptionField.placeholder = "Please tell us more before sending", this.descriptionField.value = "";
      return;
    }
    const e = {
      description: this.description,
      email: this.email,
      type: this.type
    };
    y.emit("system-info-with-callback", {
      callback: (t) => ve(`${he}feedback`, { ...e, versions: t }),
      notify: false
    }), (_a = this.parentNode) == null ? void 0 : _a.style.setProperty("--section-height", "150px"), this.message = "Thank you for sharing feedback.";
  }
  keyDown(e) {
    (e.key === "Backspace" || e.key === "Delete") && e.stopPropagation();
  }
  openGithub(e, t) {
    var _a, _b;
    if (this.type === "idea") {
      window.open(`${u}${T}`);
      return;
    }
    const l = e.replace(/\n/g, "%0A"), n = `${(_a = t.items.find((r) => r.value === this.type)) == null ? void 0 : _a.ghTitle}`, a = t.description !== "" ? t.description : F, s = D.replace("{description}", a).replace("{versionsInfo}", l);
    (_b = window.open(`${u}?title=${n}&body=${s}`, "_blank")) == null ? void 0 : _b.focus();
  }
};
o([
  b()
], i.prototype, "description", 2);
o([
  b()
], i.prototype, "type", 2);
o([
  b()
], i.prototype, "email", 2);
o([
  b()
], i.prototype, "message", 2);
o([
  b()
], i.prototype, "items", 2);
o([
  h("vaadin-text-area")
], i.prototype, "descriptionField", 2);
i = o([
  ol("copilot-feedback-panel")
], i);
const E = su({
  header: "Help Us Improve!",
  tag: "copilot-feedback-panel",
  width: 500,
  height: 500,
  floatingPosition: {
    top: 50,
    left: 50
  }
}), _ = {
  init(e) {
    e.addPanel(E);
  }
};
window.Vaadin.copilot.plugins.push(_);
export {
  i as CopilotFeedbackPanel
};
