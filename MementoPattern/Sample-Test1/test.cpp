#include "pch.h"
#include "../MementoPattern/PictureEditor.h"
#include "../MementoPattern/Picture.h"
#include "../MementoPattern/Snapshot.h"
#include "../MementoPattern/State.h"

TEST(TestCaseName, TestPictureEditor) {

	State state(0, 0, 0);
	State state2(-1, -100, 20);
	Picture pic("pic", state);
	PictureEditorp editor=make_shared<PictureEditor>(shared_ptr<Picture>(&pic));
	editor->change(state2);
	editor->cancel();
	Picture pic2 = editor->getPicture();
	
	EXPECT_EQ(pic2.getState(), state2);
}