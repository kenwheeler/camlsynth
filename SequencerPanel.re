open Revery;
open Revery.UI;
open Revery.UI.Components;
open AppState;

let component = React.component("SequencerPanel");

let createElement =
    (
      ~appState,
      ~parentDispatch,
      ~children as _: list(React.syntheticElement),
      (),
    ) =>
  component(hooks => {
    let updateStep = (index, ()) => {
      parentDispatch(UpdateStep(index));
    };

    let stepContainer =
      Style.[
        padding(5),
        backgroundColor(Color.hex("#25231E")),
        margin(5),
        flexGrow(1),
        alignSelf(`Stretch),
        flexDirection(`Row),
        alignItems(`Center),
      ];

    let stepGroup =
      Style.[
        flexGrow(1),
        margin(4),
        alignSelf(`Stretch),
        flexDirection(`Row),
        justifyContent(`SpaceBetween),
      ];

    let currentSteps = appState.tracks[appState.activeTrack].steps;

    (
      hooks,
      <View style=stepContainer>
        <View style=stepGroup>
          ...{Array.to_list(
            Array.mapi(
              (i, s) => {
                let index = 0 + i;
                <Step
                  color=Red
                  active={
                    s === 1
                      ? appState.activeStep === index ? false : true
                      : appState.activeStep === index ? true : false
                  }
                  onClick={updateStep(index)}
                />;
              },
              Array.sub(currentSteps, 0, 4),
            ),
          )}
        </View>
        <View style=stepGroup>
          ...{Array.to_list(
            Array.mapi(
              (i, s) => {
                let index = 4 + i;
                <Step
                  color=Orange
                  active={
                    s === 1
                      ? appState.activeStep === index ? false : true
                      : appState.activeStep === index ? true : false
                  }
                  onClick={updateStep(index)}
                />;
              },
              Array.sub(currentSteps, 4, 4),
            ),
          )}
        </View>
        <View style=stepGroup>
          ...{Array.to_list(
            Array.mapi(
              (i, s) => {
                let index = 8 + i;
                <Step
                  color=Yellow
                  active={
                    s === 1
                      ? appState.activeStep === index ? false : true
                      : appState.activeStep === index ? true : false
                  }
                  onClick={updateStep(index)}
                />;
              },
              Array.sub(currentSteps, 8, 4),
            ),
          )}
        </View>
        <View style=stepGroup>
          ...{Array.to_list(
            Array.mapi(
              (i, s) => {
                let index = 12 + i;
                <Step
                  color=White
                  active={
                    s === 1
                      ? appState.activeStep === index ? false : true
                      : appState.activeStep === index ? true : false
                  }
                  onClick={updateStep(index)}
                />;
              },
              Array.sub(currentSteps, 12, 4),
            ),
          )}
        </View>
      </View>,
    );
  });