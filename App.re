open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;
open Step;
let init = app => {
  let win =
    App.createWindow(
      app,
      "Reason Composer ML-808",
      ~createOptions={
        ...WindowCreateOptions.default,
        resizable: false,
        width: 800,
        height: 250,
        backgroundColor: Color.hex("#25231E"),
      },
    );

  let element = <DrumMachine />;

  let _ = UI.start(win, element);
  ();
};

App.start(init);