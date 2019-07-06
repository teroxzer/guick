/////////////////////////////////////////////////////////////////////////////
//
//	teroxGuick
//
//	copyright (C++) 2019 public domain
//
//	version 2019.07.06
//
/////////////////////////////////////////////////////////////////////////////

#include <webui/spartaWebUi.hxx>

namespace terox::guick
{

/////////////////////////////////////////////////////////////////////////////

class Guick final
{
public:

	Guick();

private:

	auto updateBoard(UiPoint) -> void;

	struct Self final
	{
		UiFrame  frame   {};
		UiLabel  title   { "teroxGuick" };
		UiLine   hline   {};
		UiGraph  board   {};
		UiLabel  help    { "Click to add or toggle a point" };
		UiLine   fline   {};
		UiLabel  count   {};
		UiButton discard { "Discard" };
		UiButton close   { "Close"   };

		Vector<UiCircle> circles {};
	}
	self;
};

/////////////////////////////////////////////////////////////////////////////

Guick::Guick()
{
	self.frame =
	{
		self.title   .geometry(  1,   3,   4,  -3 ),
		self.hline   .geometry(  4,   3,   4,  -3 ),
		self.board   .geometry(  5,   3,  -4,  -3 ),
		self.help    .geometry( -4,   3,  -3,  -3 ),
		self.fline   .geometry( -3,   3,  -2,  -3 ),
		self.count   .geometry( -2,   3,  -1, -17 ),
		self.discard .geometry( -2, -16,  -1, -10 ),
		self.close   .geometry( -2,  -9,  -1,  -3 ),
	};

	self.title.onClick = [] { UiApplication::toggleFullScreen(); };
	self.close.onClick = [] { UiApplication::exit();             };

	self.board.backColor (kUiColorCornFlowerBlue);
	self.board.color     (kUiColorLightSkyBlue);

	self.board.onPoint = [this](UiPoint point)
	{
		self.board.begin(), updateBoard(point), self.board.end();
	};

	self.discard.onClick = [this]
	{
		~self.circles;

		 self.board.clear();
		 self.count.clear();
	};
}

/////////////////////////////////////////////////////////////////////////////

auto Guick::updateBoard(UiPoint point) -> void
{
	auto constexpr radius = 8;
	auto const     size   = self.circles.size();

	for(auto i = 0; i < self.circles.size(); i++)
	{
		if(self.circles[i].contains(point))
		{
			self.circles.remove(i--);
			continue;
		}
		
		self.board.draw(self.circles[i]);
	}

	if(self.circles.size() == size)
	{
		auto circle   = circleOf(point, radius);
		self.circles += circle;
		self.board.draw(circle);
	}

	self.count = format("Count: %", self.circles.size());
}

/////////////////////////////////////////////////////////////////////////////

} // namespace terox::guick

/////////////////////////////////////////////////////////////////////////////

auto webMain() -> void
{
	UiApplication::run<terox::guick::Guick>();
}

/////////////////////////////////////////////////////////////////////////////

