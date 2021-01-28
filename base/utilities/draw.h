#pragma once
// used: std::function
#include <functional>
// used: std::deque
#include <deque>
// used: std::unique_lock
#include <mutex>
// used: std::shared_mutex
#include <shared_mutex>

// used: winapi, directx, imgui, fmt includes
#include "../common.h"
// used: color
#include "../sdk/datatypes/color.h"
// used: vector
#include "../sdk/datatypes/vector.h"

#pragma region draw_enumerations
enum class EDrawType : int
{
	NONE = 0,
	LINE,
	RECT,
	RECT_MULTICOLOR,
	CIRCLE,
	TRIANGLE,
	POLYGON,
	TEXT,
	IMAGE
};

struct DrawObject_t
{
	DrawObject_t(const EDrawType nType, std::any&& pObject) :
		nType(nType), pObject(std::move(pObject)) { }

	EDrawType nType = EDrawType::NONE;
	std::any pObject = { };
};

#pragma region draw_objects_enumerations
enum ERectRenderFlags : unsigned int
{
	IMGUI_RECT_NONE =		0,
	IMGUI_RECT_OUTLINE =	(1 << 0),
	IMGUI_RECT_BORDER =		(1 << 1),
	IMGUI_RECT_FILLED =		(1 << 2)
};

enum ECircleRenderFlags : unsigned int
{
	DRAW_CIRCLE_NONE = 0,
	DRAW_CIRCLE_OUTLINE = (1 << 0),
	DRAW_CIRCLE_FILLED = (1 << 1)
};

enum ETriangleRenderFlags : unsigned int
{
	DRAW_TRIANGLE_NONE = 0,
	DRAW_TRIANGLE_OUTLINE = (1 << 0),
	DRAW_TRIANGLE_FILLED = (1 << 1)
};

enum EPolygonRenderFlags : unsigned int
{
	DRAW_POLYGON_NONE = 0,
	DRAW_POLYGON_OUTLINE = (1 << 0),
	DRAW_POLYGON_FILLED = (1 << 1)
};

enum ETextRenderFlags : unsigned int
{
	DRAW_TEXT_NONE = 0,
	DRAW_TEXT_DROPSHADOW = (1 << 0),
	DRAW_TEXT_OUTLINE = (1 << 1)
};
#pragma endregion

#pragma region draw_objects_structures
struct LineObject_t
{
	ImVec2 vecStart = { };
	ImVec2 vecEnd = { };
	ImU32 colLine = 0x0;
	float flThickness = 0.f;
};

struct RectObject_t
{
	IMGUI_CIRCLE_NONE =		0,
	IMGUI_CIRCLE_OUTLINE =	(1 << 0),
	IMGUI_CIRCLE_FILLED =	(1 << 1)
};

// triangle rendering flags
enum ETriangleRenderFlags : int
{
	ImVec2 vecMin = { };
	ImVec2 vecMax = { };
	ImU32 colUpperLeft = 0x0;
	ImU32 colUpperRight = 0x0;
	ImU32 colBottomRight = 0x0;
	ImU32 colBottomLeft = 0x0;
};
#pragma endregion

struct DrawObject_t
{
	EDrawType nType = EDrawType::NONE;

struct TriangleObject_t
{
	ImVec2 vecFirst = { };
	ImVec2 vecSecond = { };
	ImVec2 vecThird = { };
	ImU32 colTriangle = 0x0;
	unsigned int uFlags = 0x0;
	ImU32 colOutline = 0x0;
	float flThickness = 0.f;
};

struct PolygonObject_t
{
	std::vector<ImVec2> vecPoints = { };
	ImU32 colPolygon = 0x0;
	unsigned int uFlags = 0x0;
	ImU32 colOutline = 0x0;
	bool bClosed = false;
	float flThickness = 0.f;
};

struct TextObject_t
{
	const ImFont* pFont = nullptr;
	float flFontSize = 0.f;
	ImVec2 vecPosition = { };
	std::string szText = { };
	int iFlags = 0;

	/* primitive factors */
	float flRadius = 0.0f;
	int nSegments = 0;
	float flRounding = 0.0f;
	ImDrawCornerFlags roundingCorners = ImDrawCornerFlags_None;
	float flThickness = 0.0f;
};

/*
 * FONTS
 */
namespace F
{
	/*
	 * fonts navigation:
	 * [N][purpose]	[max size]	[flags]
	 */

	// 0	main		13		autohinting
	inline ImFont* Whitney;
	// 1	extra		14		bold
	inline ImFont* Verdana;
	// 2	visuals		40		lighthinting
	inline ImFont* SmallestPixel;
	// 3	icons		40		lighthinting
	inline ImFont* Icons;
}

// extended imgui functionality
namespace ImGui
{
	// Main
	void HelpMarker(const char* szDescription);
	bool ListBox(const char* szLabel, int* iCurrentItem, std::function<const char* (int)> pLambda, int nItemsCount, int iHeightInItems);
	bool HotKey(const char* szLabel, int* v);
	bool MultiCombo(const char* szLabel, const char** szDisplayName, std::vector<bool>& v, int nHeightInItems);

	// Wrappers
	bool Combo(const char* szLabel, std::vector<int>& v, int nIndex, const char* szItemsSeparatedByZeros, int nHeightInItems = -1);
	bool Checkbox(const char* szLabel, std::vector<bool>& v, int nIndex);
	bool SliderFloat(const char* szLabel, std::vector<float>& v, int nIndex, float flMin, float flMax, const char* szFormat = "%.3f", float flPower = 1.0f);
	bool SliderInt(const char* szLabel, std::vector<int>& v, int nIndex, int iMin, int iMax, const char* szFormat = "%d");
	bool ColorEdit3(const char* szLabel, Color* v, ImGuiColorEditFlags flags);
	bool ColorEdit4(const char* szLabel, Color* v, ImGuiColorEditFlags flags);
}

/*
 * DRAW
 * ready rendering framework
 */
namespace D
{
	// Get
	/* create fonts, set styles etc */
	void Setup(IDirect3DDevice9* pDevice, unsigned int uFontFlags = 0x0);
	/* shutdown imgui */
	void Destroy();
	/* render primitives by stored & safe data */
	void RenderDrawData(ImDrawList* pDrawList);
	/* clear native data to draw */
	void ClearDrawData();
	/* swap native draw data to safe */
	void SwapDrawData();

	// Render
	void AddLine(const ImVec2& vecStart, const ImVec2& vecEnd, const Color& colLine, float flThickness = 1.0f);
	void AddRect(const ImVec2& vecMin, const ImVec2& vecMax, const Color& colRect, unsigned int uFlags = DRAW_RECT_NONE, const Color& colOutline = Color(0, 0, 0, 255), float flRounding = 0.f, ImDrawCornerFlags roundingCorners = ImDrawCornerFlags_All, float flThickness = 1.0f);
	void AddRectMultiColor(const ImVec2& vecMin, const ImVec2& vecMax, const Color& colUpperLeft, const Color& colUpperRight, const Color& colBottomRight, const Color& colBottomLeft);
	void AddCircle(const ImVec2& vecCenter, float flRadius, const Color& colCircle, int nSegments = 12, unsigned int uFlags = DRAW_CIRCLE_NONE, const Color& colOutline = Color(0, 0, 0, 255), float flThickness = 1.0f);
	void AddTriangle(const ImVec2& vecFirst, const ImVec2& vecSecond, const ImVec2& vecThird, const Color& colTriangle, unsigned int uFlags = DRAW_TRIANGLE_NONE, const Color& colOutline = Color(0, 0, 0, 255), float flThickness = 1.0f);
	void AddPolygon(std::vector<ImVec2>& vecPoints, const Color& colPolygon, unsigned int uFlags = DRAW_POLYGON_FILLED, const Color& colOutline = Color(0, 0, 0, 255), bool bClosed = true, float flThickness = 1.0f);
	void AddText(const ImFont* pFont, float flFontSize, const ImVec2& vecPosition, const std::string& szText, const Color& colText, unsigned int uFlags = DRAW_TEXT_NONE, const Color& colOutline = Color(0, 0, 0, 255));
	void AddText(const ImVec2& vecPosition, const std::string& szText, const Color& colText, int iFlags = DRAW_TEXT_NONE, const Color& colOutline = Color(0, 0, 0, 255));
	void AddImage(ImTextureID pTexture, const ImVec2& vecMin, const ImVec2& vecMax, const Color& colImage = Color(255, 255, 255, 255), float flRounding = 0.f, ImDrawCornerFlags roundingCorners = ImDrawCornerFlags_All);

	// Bindings
	void AddDrawListRect(ImDrawList* pDrawList, const ImVec2& vecMin, const ImVec2& vecMax, ImU32 colRect, unsigned int uFlags = DRAW_RECT_NONE, ImU32 colOutline = IM_COL32(0, 0, 0, 255), float flRounding = 0.f, ImDrawCornerFlags roundingCorners = ImDrawCornerFlags_All, float flThickness = 1.0f);
	void AddDrawListText(ImDrawList* pDrawList, const ImFont* pFont, float flFontSize, const ImVec2& vecPosition, const std::string& szText, ImU32 colText, unsigned int uFlags = DRAW_TEXT_NONE, ImU32 colOutline = IM_COL32(0, 0, 0, 255));

	// Extra
	/* converts 3d game world space to screen space */
	bool WorldToScreen(const Vector& vecOrigin, ImVec2& vecScreen);

	// Values
	/* directx init state */
	inline bool	bInitialized = false;
	/* saved data to draw */
	inline std::deque<DrawObject_t> vecDrawData = { };
	/* thread-safe data to draw */
	inline std::deque<DrawObject_t> vecSafeDrawData = { };
	/* thread-safe render mutex */
	inline std::shared_mutex drawMutex = { };
}
