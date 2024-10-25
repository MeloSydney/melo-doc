

//TODO - vscode plugin

code anchor

comment anchors

dracula official

markdown all in one

todo tree


//TODO - indentation && display size

{
	"editor.tabSize": 8,
	"editor.insertSpaces": false
}


//TODO - vim config in user settings

{
	// "workbench.colorTheme": "Visual Studio Dark",
	"workbench.colorTheme": "Oceanic Plus",
	// 文件打开 非预览模式 不用双击
	"workbench.editor.enablePreviewFromQuickOpen": false,
	// 空白渲染
	"editor.renderWhitespace": "all",
	"security.workspace.trust.untrustedFiles": "open",
	"remote.SSH.remotePlatform": {
		"100.82.100.28": "linux",
		"30.72.114.100": "linux"
	},
	"editor.wordWrap": "on",
	// 字体
	"editor.fontSize": 16,
	// format后 xx
	"editor.formatOnSave": true,
	"editor.formatOnPaste": true,
	// 字母间距加大
	"editor.letterSpacing": 1,
	// 删除结尾空格
	"files.trimTrailingWhitespace": true,
	// "clangd.path": "/usr/bin/clangd",
	// "clangd.arguments": [
	// 	"--log=verbose",
	// ],
	// 缩进配置
	"editor.tabSize": 8,
	// 使用tab 缩进
	"editor.detectIndentation": false,
	// 禁用
	"editor.minimap.enabled": false,
	"C_Cpp.clang_format_fallbackStyle": "Google",
	// "C_Cpp.clang_format_path": "/home/shifan/src-np1/hypervisor/native/.clang-format",
	"C_Cpp.clang_format_style": "/home/shifan/src-np1/hypervisor/native/.clang-format",
	"editor.lineNumbers": "relative",
	// 禁用 参数提示
	"editor.parameterHints.enabled": false,
	"editor.quickSuggestions": {
		"comments": "off",
		"strings": "off",
		"other": "off"
	},
	// "workbench.list.automaticKeyboardNavigation": false,
	"vim.leader": "<space>",
	// "vim.leader": ",",
	"vim.easymotion": true,
	"vim.incsearch": true,
	// 使用 系统剪切板
	"vim.useSystemClipboard": true,
	"vim.useCtrlKeys": true,
	// search 配置
	"vim.hlsearch": true,
	"vim.searchHighlightColor": "#5f00af",
	// vscode 处理快捷键 而并非vim
	"vim.handleKeys": {
		"<C-a>": false,
		"<C-f>": false,
		"<C-x>": false,
		"<C-c>": false,
		"<C-v>": false,
		"<C-z>": false,
		"<C-o>": false,
		"<C-p>": false,
		"<C-w>": false,
		"<C-n>": false,
	},
	"vim.insertModeKeyBindings": [],
	"vim.normalModeKeyBindingsNonRecursive": [
		// leader+u/d 代替 Ctrl+u/d 使页面向上/下滚动半屏
		{
			"before": [
				"leader",
				"u"
			],
			"after": [
				"<C-u>"
			]
		},
		{
			"before": [
				"<leader>",
				"c"
			],
			"commands": [
				"editor.action.commentLine",
			],
		},
		{
			"before": [
				"leader",
				"d"
			],
			"after": [
				"<C-d>"
			]
		},
		// 左右split
		{
			"before": [
				"<leader>",
				"w"
			],
			"commands": [
				"workbench.action.splitEditor"
			]
		},
		{
			"before": [
				"<leader>",
				"j"
			],
			"after": [
				"9",
				"j"
			]
		},
		{
			"before": [
				"<leader>",
				"k"
			],
			"after": [
				"9",
				"k"
			]
		},
	],
	"vim.commandLineModeKeyBindingsNonRecursive": [],
	"vim.operatorPendingModeKeyBindings": [],
	"[c]": {
		"editor.defaultFormatter": "ms-vscode.cpptools"
	},
	"search.exclude": {
		"**/1.info": true,
	},
	"files.defaultLanguage": "C",
	"files.autoSave": "afterDelay",
	"files.autoSaveDelay": 500,
}


//TODO - keybingings.json

// Place your key bindings in this file to override the defaultsauto[]
[
	{
		"key": "ctrl+shift+alt+v",
		"command": "telesoho.MarkdownPaste",
		"when": "editorTextFocus && resourceLangId == 'markdown'"
	},
	{
		"key": "ctrl+alt+v",
		"command": "-telesoho.MarkdownPaste",
		"when": "editorTextFocus && resourceLangId == 'markdown'"
	},
	{
		"key": "ctrl+shift+left",
		"command": "-cursorWordLeftSelect",
		"when": "textInputFocus && !accessibilityModeEnabled"
	},
	{
		"key": "ctrl+shift+left",
		"command": "search.action.collapseSearchResults"
	},
	// 打开一个目录
	{
		"key": "ctrl+g m",
		"command": "workbench.action.files.openFolder"
	},
	// 打开一个文件
	{
		"key": "ctrl+g f",
		"command": "workbench.action.files.openFile"
	},
	// 格式化代码
	{
		"key": "ctrl+alt+l",
		"command": "editor.action.formatDocument",
		"when": "editorTextFocus && !editorReadonly"
	},
	{ // 切换上一个编辑器
		"key": "alt+k",
		"command": "workbench.action.previousEditor"
	},
	{ // 切换下一个编辑器
		"key": "alt+l",
		"command": "workbench.action.nextEditor"
	},
	{ // 跳转到引用
		"key": "alt+r",
		"command": "editor.action.goToReferences",
		"when": "editorHasReferenceProvider && editorTextFocus && !inReferenceSearchEditor && !isInEmbeddedEditor"
	},
	{ // 跳转到实现
		"key": "alt+i",
		"command": "editor.action.goToImplementation",
		"when": "editorHasImplementationProvider && editorTextFocus && !isInEmbeddedEditor"
	},
	{ // 跳转到定义
		"key": "alt+d",
		"command": "editor.action.revealDefinition",
		"when": "editorHasDefinitionProvider && editorTextFocus && !isInEmbeddedEditor"
	},
	{ // 跳转到类型定义
		"key": "alt+shift+d",
		"command": "editor.action.goToTypeDefinition",
		"when": "editorHasReferenceProvider && editorTextFocus && !inReferenceSearchEditor && !isInEmbeddedEditor"
	},
]


//TODO - refs

https://www.cnblogs.com/YunyaSir/p/15522565.html
https://browser.alibaba-inc.com/?Url=https://hanzhen.wang/posts/vscode-vim


gd              跳转定义
ctrl+]
ctrl+o          回跳
gcc             注释一行
<<              左移
>>              右移
shift+v         多行模式
gt              向后一个标签
gT              向前一个标签



//TODO - ansi color

1 ansi color 插安装件
2 Ctrl shift p
3 ansi text: open preview




//TODO - c/cpp 关闭语法检查

errorSquiggles disable