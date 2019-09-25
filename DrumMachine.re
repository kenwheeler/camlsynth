open Revery;
open Revery.UI;
open Revery.UI.Components;
open Audio;
open Thread;
open Portaudio;
open AppState;

type threadArgs = {
  dispatch: action => unit,
  appStateRef: ref(appState),
};

let component = React.component("DrumMachine");

type streamParams =
  option(Portaudio.stream_parameters(float, Bigarray.float32_elt));

let stream:
  Portaudio.stream(streamParams, streamParams, float, Bigarray.float32_elt) =
  getStream();

let appStateRef = ref(initialAppState);

let createElement = (~children as _, ()) =>
  component(hooks => {
    let (appState, dispatch, hooks) =
      Hooks.reducer(~initialState=initialAppState, reducer, hooks);

    let hooks =
      Hooks.effect(
        Always,
        () => {
          appStateRef := appState;
          None;
        },
        hooks,
      );

    let setActiveStep = step => {
      dispatch(SetActiveStep(step - 1));
    };

    let updateStep = (index, ()) => {
      dispatch(UpdateStep(index));
    };

    let updateTrack = (track, ()) => {
      dispatch(SetActiveTrack(track));
    };

    let updateTempo = (tempo, ()) => {
      dispatch(UpdateTempo(tempo));
    };

    let playToggle = () => {
      if (appState.playing) {
        setActiveStep(0);
        Audio.stop(stream);
        dispatch(SetPlaying(false));
      } else {
        start_stream(stream);
        let _handle =
          Thread.create(
            ({dispatch, appStateRef}) =>
              Audio.play(stream, dispatch, appStateRef),
            {dispatch, appStateRef},
          );
        dispatch(SetPlaying(true));
      };

      ();
    };

    let containerStyle =
      Style.[
        position(`Absolute),
        justifyContent(`Center),
        alignItems(`Center),
        bottom(0),
        top(0),
        left(0),
        right(0),
      ];

    let innerStyle =
      Style.[
        flexGrow(0),
        alignSelf(`Stretch),
        flexDirection(`Row),
        alignItems(`FlexEnd),
        backgroundColor(Color.hex("#888888")),
        margin(20),
        height(100),
        borderRadius(5.),
        border(~width=1, ~color=Color.hex("#888888")),
      ];
    let settingsContainer =
      Style.[
        flexGrow(1),
        alignSelf(`Stretch),
        flexDirection(`Row),
        alignItems(`Center),
        paddingHorizontal(10),
        paddingVertical(5),
        backgroundColor(Color.hex("#888888")),
        marginHorizontal(20),
        marginBottom(20),
        marginTop(-10),
        borderRadius(5.),
        justifyContent(`SpaceBetween),
        border(~width=1, ~color=Color.hex("#888888")),
      ];
    let headerStyle =
      Style.[
        flexDirection(`Row),
        justifyContent(`FlexEnd),
        alignSelf(`Stretch),
        paddingTop(20),
        paddingRight(20),
        borderBottom(~width=1, ~color=Color.hex("#F16F20")),
      ];
    let textHeaderStyle =
      Style.[
        color(Color.hex("#F16F20")),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(24),
      ];
    let sliderLabelStyle =
      Style.[
        color(Colors.black),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(12),
      ];
    let subTextStyle =
      Style.[
        color(Color.hex("#F16F20")),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(18),
        marginLeft(20),
        marginTop(6),
      ];
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

    let trackStyle =
      Style.[
        alignItems(`FlexStart),
        flexDirection(`Row),
        flexGrow(1),
        marginLeft(20),
        justifyContent(`FlexStart),
      ];

    let currentSteps = appState.tracks[appState.activeTrack].steps;

    (
      hooks,
      <View style=containerStyle>
        <View style=headerStyle>
          <View style=trackStyle>
            <TrackButton
              onClick={updateTrack(0)}
              active={appState.activeTrack == 0}
              text="KCK"
            />
            <TrackButton
              onClick={updateTrack(1)}
              active={appState.activeTrack == 1}
              text="SNR"
            />
            <TrackButton
              onClick={updateTrack(2)}
              active={appState.activeTrack == 2}
              text="HAT"
            />
            <TrackButton
              onClick={updateTrack(3)}
              active={appState.activeTrack == 3}
              text="CBL"
            />
            <TrackButton
              onClick={updateTrack(4)}
              active={appState.activeTrack == 4}
              text="CLV"
            />
          </View>
          <TempoControl onChange=updateTempo tempo={appState.tempo} />
          <Text style=textHeaderStyle text="Reason Composer" />
          <Text style=subTextStyle text="ML-808" />
        </View>
        <View style=innerStyle>
          <PlayButton playing={appState.playing} onClick=playToggle />
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
          </View>
        </View>
        <View style=settingsContainer>
          <View>
            <Knob
              onValueChanged={v => dispatch(SetAttack(v))}
              value={appState.tracks[appState.activeTrack].attack}
              maximumValue=0.5
              minimumValue=0.01
            />
            <Center> <Text style=sliderLabelStyle text="ATTACK" /> </Center>
          </View>
          <View>
            <Knob
              onValueChanged={v => dispatch(SetDecay(v))}
              value={appState.tracks[appState.activeTrack].decay}
              maximumValue=0.99
              minimumValue=0.01
            />
            <Center> <Text style=sliderLabelStyle text="DECAY" /> </Center>
          </View>
          <View>
            <Knob
              onValueChanged={v => dispatch(SetSustain(v))}
              value={appState.tracks[appState.activeTrack].sustain}
              maximumValue=0.99
              minimumValue=0.01
            />
            <Center> <Text style=sliderLabelStyle text="SUSTAIN" /> </Center>
          </View>
          <View>
            <Knob
              onValueChanged={v => dispatch(SetRelease(v))}
              value={appState.tracks[appState.activeTrack].release}
              maximumValue=2.0
              minimumValue=0.01
            />
            <Center> <Text style=sliderLabelStyle text="RELEASE" /> </Center>
          </View>
          <View>
            <Knob
              onValueChanged={v => dispatch(SetFreq(v))}
              value={appState.tracks[appState.activeTrack].freq}
              maximumValue=130.81
              minimumValue=16.35
            />
            <Center> <Text style=sliderLabelStyle text="TONE" /> </Center>
          </View>
          <View>
            <Knob
              onValueChanged={v => dispatch(SetGain(v))}
              value={appState.tracks[appState.activeTrack].gain}
              maximumValue=1.5
              minimumValue=0.0001
            />
            <Center> <Text style=sliderLabelStyle text="GAIN" /> </Center>
          </View>
        </View>
      </View>,
    );
  });