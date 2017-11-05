/*
This file is part of Telegram Desktop,
the official desktop version of Telegram messaging app, see https://telegram.org

Telegram Desktop is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

It is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

In addition, as a special exception, the copyright holders give permission
to link the code of portions of this program with the OpenSSL library.

Full license: https://github.com/telegramdesktop/tdesktop/blob/master/LICENSE
Copyright (c) 2014-2017 John Preston, https://desktop.telegram.org
*/
#pragma once

#include <rpl/producer.h>
#include "ui/rp_widget.h"

namespace Window {
class Controller;
} // namespace Window

namespace Ui {
class VerticalLayout;
template <typename Widget>
class SlideWrap;
struct ScrollToRequest;
} // namespace Ui

namespace Info {

enum class Wrap;
class Controller;

namespace Profile {

class Memento;
class Members;
class Cover;

class InnerWidget final : public Ui::RpWidget {
public:
	InnerWidget(
		QWidget *parent,
		not_null<Controller*> controller);

	void saveState(not_null<Memento*> memento);
	void restoreState(not_null<Memento*> memento);

	void setIsStackBottom(bool isStackBottom) {
		_isStackBottom.fire_copy(isStackBottom);
	}
	rpl::producer<Ui::ScrollToRequest> scrollToRequests() const {
		return _scrollToRequests.events();
	}

	rpl::producer<int> desiredHeightValue() const override {
		return _desiredHeight.events_starting_with(countDesiredHeight());
	}

protected:
	int resizeGetHeight(int newWidth) override;
	void visibleTopBottomUpdated(
		int visibleTop,
		int visibleBottom) override;

private:
	object_ptr<RpWidget> setupContent(RpWidget *parent);
	object_ptr<RpWidget> setupDetails(RpWidget *parent) const;
	object_ptr<RpWidget> setupSharedMedia(RpWidget *parent);
	object_ptr<RpWidget> setupMuteToggle(RpWidget *parent) const;
	object_ptr<RpWidget> setupInfo(RpWidget *parent) const;
	void setupUserButtons(
		Ui::VerticalLayout *wrap,
		not_null<UserData*> user) const;
	object_ptr<RpWidget> setupUserActions(
		RpWidget *parent,
		not_null<UserData*> user) const;
	void shareContact(not_null<UserData*> user) const;

	object_ptr<RpWidget> createSkipWidget(RpWidget *parent) const;
	object_ptr<Ui::SlideWrap<RpWidget>> createSlideSkipWidget(
		RpWidget *parent) const;

	int countDesiredHeight() const;

	bool canHideDetailsEver() const;
	rpl::producer<bool> canHideDetails() const;

	rpl::event_stream<bool> _isStackBottom;

	const not_null<Controller*> _controller;
	const not_null<PeerData*> _peer;
	PeerData * const _migrated = nullptr;

	Members *_members = nullptr;
	Cover *_cover = nullptr;
	Ui::SlideWrap<RpWidget> *_infoWrap = nullptr;
	Ui::SlideWrap<RpWidget> *_sharedMediaWrap = nullptr;
	object_ptr<RpWidget> _content;

	rpl::event_stream<Ui::ScrollToRequest> _scrollToRequests;
	rpl::event_stream<int> _desiredHeight;

};

} // namespace Profile
} // namespace Info