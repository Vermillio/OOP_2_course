//Simple Visitor pattern example

#include "common.h"
#include "PictureEditor.h"
#include "Picture.h"
#include "Snapshot.h"
#include "State.h"


int
main ()
{
  cout << " ---Memento pattern demo--- " << endl;
  int sum = 0;
  Picturep picture = make_shared < Picture > ("MyPicture", State(0,0,0));
  PictureEditor editor (picture);
  vector <State> states = { {1, 5, 90}, {34, -29, 150}, {34, -512, 4} };
  for (size_t i = 0; i < states.size (); ++i)
    {
      editor.change (states[i]);
      cout << picture->to_json() << endl;
      editor.cancel();
      cout << picture->to_json() << endl;
    }
  system ("pause");
  return 0;
}
