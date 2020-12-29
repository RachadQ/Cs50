/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See License.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
define(["require", "exports", "vs/base/browser/dom", "vs/base/browser/ui/sash/sash", "vs/base/common/color", "vs/base/common/idGenerator", "vs/base/common/lifecycle", "vs/base/common/objects", "vs/editor/common/core/range", "vs/editor/common/model/textModel", "vs/css!./zoneWidget"], function (require, exports, dom, sash_1, color_1, idGenerator_1, lifecycle_1, objects, range_1, textModel_1) {
    "use strict";
    Object.defineProperty(exports, "__esModule", { value: true });
    const defaultColor = new color_1.Color(new color_1.RGBA(0, 122, 204));
    const defaultOptions = {
        showArrow: true,
        showFrame: true,
        className: '',
        frameColor: defaultColor,
        arrowColor: defaultColor,
        keepEditorSelection: false
    };
    const WIDGET_ID = 'vs.editor.contrib.zoneWidget';
    class ViewZoneDelegate {
        constructor(domNode, afterLineNumber, afterColumn, heightInLines, onDomNodeTop, onComputedHeight) {
            this.id = ''; // A valid zone id should be greater than 0
            this.domNode = domNode;
            this.afterLineNumber = afterLineNumber;
            this.afterColumn = afterColumn;
            this.heightInLines = heightInLines;
            this._onDomNodeTop = onDomNodeTop;
            this._onComputedHeight = onComputedHeight;
        }
        onDomNodeTop(top) {
            this._onDomNodeTop(top);
        }
        onComputedHeight(height) {
            this._onComputedHeight(height);
        }
    }
    exports.ViewZoneDelegate = ViewZoneDelegate;
    class OverlayWidgetDelegate {
        constructor(id, domNode) {
            this._id = id;
            this._domNode = domNode;
        }
        getId() {
            return this._id;
        }
        getDomNode() {
            return this._domNode;
        }
        getPosition() {
            return null;
        }
    }
    exports.OverlayWidgetDelegate = OverlayWidgetDelegate;
    class Arrow {
        constructor(_editor) {
            this._editor = _editor;
            this._ruleName = Arrow._IdGenerator.nextId();
            this._decorations = [];
            this._color = null;
            this._height = -1;
            //
        }
        dispose() {
            this.hide();
            dom.removeCSSRulesContainingSelector(this._ruleName);
        }
        set color(value) {
            if (this._color !== value) {
                this._color = value;
                this._updateStyle();
            }
        }
        set height(value) {
            if (this._height !== value) {
                this._height = value;
                this._updateStyle();
            }
        }
        _updateStyle() {
            dom.removeCSSRulesContainingSelector(this._ruleName);
            dom.createCSSRule(`.monaco-editor ${this._ruleName}`, `border-style: solid; border-color: transparent; border-bottom-color: ${this._color}; border-width: ${this._height}px; bottom: -${this._height}px; margin-left: -${this._height}px; `);
        }
        show(where) {
            this._decorations = this._editor.deltaDecorations(this._decorations, [{ range: range_1.Range.fromPositions(where), options: { className: this._ruleName, stickiness: 1 /* NeverGrowsWhenTypingAtEdges */ } }]);
        }
        hide() {
            this._editor.deltaDecorations(this._decorations, []);
        }
    }
    Arrow._IdGenerator = new idGenerator_1.IdGenerator('.arrow-decoration-');
    class ZoneWidget {
        constructor(editor, options = {}) {
            this._arrow = null;
            this._overlayWidget = null;
            this._resizeSash = null;
            this._positionMarkerId = [];
            this._viewZone = null;
            this._disposables = new lifecycle_1.DisposableStore();
            this.container = null;
            this._isShowing = false;
            this.editor = editor;
            this.options = objects.deepClone(options);
            objects.mixin(this.options, defaultOptions, false);
            this.domNode = document.createElement('div');
            if (!this.options.isAccessible) {
                this.domNode.setAttribute('aria-hidden', 'true');
                this.domNode.setAttribute('role', 'presentation');
            }
            this._disposables.add(this.editor.onDidLayoutChange((info) => {
                const width = this._getWidth(info);
                this.domNode.style.width = width + 'px';
                this.domNode.style.left = this._getLeft(info) + 'px';
                this._onWidth(width);
            }));
        }
        dispose() {
            if (this._overlayWidget) {
                this.editor.removeOverlayWidget(this._overlayWidget);
                this._overlayWidget = null;
            }
            if (this._viewZone) {
                this.editor.changeViewZones(accessor => {
                    if (this._viewZone) {
                        accessor.removeZone(this._viewZone.id);
                    }
                    this._viewZone = null;
                });
            }
            this.editor.deltaDecorations(this._positionMarkerId, []);
            this._positionMarkerId = [];
            this._disposables.dispose();
        }
        create() {
            dom.addClass(this.domNode, 'zone-widget');
            if (this.options.className) {
                dom.addClass(this.domNode, this.options.className);
            }
            this.container = document.createElement('div');
            dom.addClass(this.container, 'zone-widget-container');
            this.domNode.appendChild(this.container);
            if (this.options.showArrow) {
                this._arrow = new Arrow(this.editor);
                this._disposables.add(this._arrow);
            }
            this._fillContainer(this.container);
            this._initSash();
            this._applyStyles();
        }
        style(styles) {
            if (styles.frameColor) {
                this.options.frameColor = styles.frameColor;
            }
            if (styles.arrowColor) {
                this.options.arrowColor = styles.arrowColor;
            }
            this._applyStyles();
        }
        _applyStyles() {
            if (this.container && this.options.frameColor) {
                let frameColor = this.options.frameColor.toString();
                this.container.style.borderTopColor = frameColor;
                this.container.style.borderBottomColor = frameColor;
            }
            if (this._arrow && this.options.arrowColor) {
                let arrowColor = this.options.arrowColor.toString();
                this._arrow.color = arrowColor;
            }
        }
        _getWidth(info) {
            return info.width - info.minimapWidth - info.verticalScrollbarWidth;
        }
        _getLeft(info) {
            // If minimap is to the left, we move beyond it
            if (info.minimapWidth > 0 && info.minimapLeft === 0) {
                return info.minimapWidth;
            }
            return 0;
        }
        _onViewZoneTop(top) {
            this.domNode.style.top = top + 'px';
        }
        _onViewZoneHeight(height) {
            this.domNode.style.height = `${height}px`;
            if (this.container) {
                let containerHeight = height - this._decoratingElementsHeight();
                this.container.style.height = `${containerHeight}px`;
                const layoutInfo = this.editor.getLayoutInfo();
                this._doLayout(containerHeight, this._getWidth(layoutInfo));
            }
            if (this._resizeSash) {
                this._resizeSash.layout();
            }
        }
        get position() {
            const [id] = this._positionMarkerId;
            if (!id) {
                return undefined;
            }
            const model = this.editor.getModel();
            if (!model) {
                return undefined;
            }
            const range = model.getDecorationRange(id);
            if (!range) {
                return undefined;
            }
            return range.getStartPosition();
        }
        show(rangeOrPos, heightInLines) {
            const range = range_1.Range.isIRange(rangeOrPos)
                ? rangeOrPos
                : new range_1.Range(rangeOrPos.lineNumber, rangeOrPos.column, rangeOrPos.lineNumber, rangeOrPos.column);
            this._isShowing = true;
            this._showImpl(range, heightInLines);
            this._isShowing = false;
            this._positionMarkerId = this.editor.deltaDecorations(this._positionMarkerId, [{ range, options: textModel_1.ModelDecorationOptions.EMPTY }]);
        }
        hide() {
            if (this._viewZone) {
                this.editor.changeViewZones(accessor => {
                    if (this._viewZone) {
                        accessor.removeZone(this._viewZone.id);
                    }
                });
                this._viewZone = null;
            }
            if (this._overlayWidget) {
                this.editor.removeOverlayWidget(this._overlayWidget);
                this._overlayWidget = null;
            }
            if (this._arrow) {
                this._arrow.hide();
            }
        }
        _decoratingElementsHeight() {
            let lineHeight = this.editor.getConfiguration().lineHeight;
            let result = 0;
            if (this.options.showArrow) {
                let arrowHeight = Math.round(lineHeight / 3);
                result += 2 * arrowHeight;
            }
            if (this.options.showFrame) {
                let frameThickness = Math.round(lineHeight / 9);
                result += 2 * frameThickness;
            }
            return result;
        }
        _showImpl(where, heightInLines) {
            const position = {
                lineNumber: where.startLineNumber,
                column: where.startColumn
            };
            const layoutInfo = this.editor.getLayoutInfo();
            const width = this._getWidth(layoutInfo);
            this.domNode.style.width = `${width}px`;
            this.domNode.style.left = this._getLeft(layoutInfo) + 'px';
            // Render the widget as zone (rendering) and widget (lifecycle)
            const viewZoneDomNode = document.createElement('div');
            viewZoneDomNode.style.overflow = 'hidden';
            const lineHeight = this.editor.getConfiguration().lineHeight;
            // adjust heightInLines to viewport
            const maxHeightInLines = (this.editor.getLayoutInfo().height / lineHeight) * 0.8;
            if (heightInLines >= maxHeightInLines) {
                heightInLines = maxHeightInLines;
            }
            let arrowHeight = 0;
            let frameThickness = 0;
            // Render the arrow one 1/3 of an editor line height
            if (this._arrow && this.options.showArrow) {
                arrowHeight = Math.round(lineHeight / 3);
                this._arrow.height = arrowHeight;
                this._arrow.show(position);
            }
            // Render the frame as 1/9 of an editor line height
            if (this.options.showFrame) {
                frameThickness = Math.round(lineHeight / 9);
            }
            // insert zone widget
            this.editor.changeViewZones((accessor) => {
                if (this._viewZone) {
                    accessor.removeZone(this._viewZone.id);
                }
                if (this._overlayWidget) {
                    this.editor.removeOverlayWidget(this._overlayWidget);
                    this._overlayWidget = null;
                }
                this.domNode.style.top = '-1000px';
                this._viewZone = new ViewZoneDelegate(viewZoneDomNode, position.lineNumber, position.column, heightInLines, (top) => this._onViewZoneTop(top), (height) => this._onViewZoneHeight(height));
                this._viewZone.id = accessor.addZone(this._viewZone);
                this._overlayWidget = new OverlayWidgetDelegate(WIDGET_ID + this._viewZone.id, this.domNode);
                this.editor.addOverlayWidget(this._overlayWidget);
            });
            if (this.container && this.options.showFrame) {
                const width = this.options.frameWidth ? this.options.frameWidth : frameThickness;
                this.container.style.borderTopWidth = width + 'px';
                this.container.style.borderBottomWidth = width + 'px';
            }
            let containerHeight = heightInLines * lineHeight - this._decoratingElementsHeight();
            if (this.container) {
                this.container.style.top = arrowHeight + 'px';
                this.container.style.height = containerHeight + 'px';
                this.container.style.overflow = 'hidden';
            }
            this._doLayout(containerHeight, width);
            if (!this.options.keepEditorSelection) {
                this.editor.setSelection(where);
            }
            const model = this.editor.getModel();
            if (model) {
                // Reveal the line above or below the zone widget, to get the zone widget in the viewport
                const revealLineNumber = Math.min(model.getLineCount(), Math.max(1, where.endLineNumber + 1));
                this.revealLine(revealLineNumber);
            }
        }
        revealLine(lineNumber) {
            this.editor.revealLine(lineNumber, 0 /* Smooth */);
        }
        setCssClass(className, classToReplace) {
            if (!this.container) {
                return;
            }
            if (classToReplace) {
                this.container.classList.remove(classToReplace);
            }
            dom.addClass(this.container, className);
        }
        _onWidth(widthInPixel) {
            // implement in subclass
        }
        _doLayout(heightInPixel, widthInPixel) {
            // implement in subclass
        }
        _relayout(newHeightInLines) {
            if (this._viewZone && this._viewZone.heightInLines !== newHeightInLines) {
                this.editor.changeViewZones(accessor => {
                    if (this._viewZone) {
                        this._viewZone.heightInLines = newHeightInLines;
                        accessor.layoutZone(this._viewZone.id);
                    }
                });
            }
        }
        // --- sash
        _initSash() {
            if (this._resizeSash) {
                return;
            }
            this._resizeSash = this._disposables.add(new sash_1.Sash(this.domNode, this, { orientation: 1 /* HORIZONTAL */ }));
            if (!this.options.isResizeable) {
                this._resizeSash.hide();
                this._resizeSash.state = 0 /* Disabled */;
            }
            let data;
            this._disposables.add(this._resizeSash.onDidStart((e) => {
                if (this._viewZone) {
                    data = {
                        startY: e.startY,
                        heightInLines: this._viewZone.heightInLines,
                    };
                }
            }));
            this._disposables.add(this._resizeSash.onDidEnd(() => {
                data = undefined;
            }));
            this._disposables.add(this._resizeSash.onDidChange((evt) => {
                if (data) {
                    let lineDelta = (evt.currentY - data.startY) / this.editor.getConfiguration().lineHeight;
                    let roundedLineDelta = lineDelta < 0 ? Math.ceil(lineDelta) : Math.floor(lineDelta);
                    let newHeightInLines = data.heightInLines + roundedLineDelta;
                    if (newHeightInLines > 5 && newHeightInLines < 35) {
                        this._relayout(newHeightInLines);
                    }
                }
            }));
        }
        getHorizontalSashLeft() {
            return 0;
        }
        getHorizontalSashTop() {
            return (this.domNode.style.height === null ? 0 : parseInt(this.domNode.style.height)) - (this._decoratingElementsHeight() / 2);
        }
        getHorizontalSashWidth() {
            const layoutInfo = this.editor.getLayoutInfo();
            return layoutInfo.width - layoutInfo.minimapWidth;
        }
    }
    exports.ZoneWidget = ZoneWidget;
});
//# sourceMappingURL=zoneWidget.js.map