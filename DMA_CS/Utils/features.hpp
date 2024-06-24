

namespace Features {

    namespace Home {
        bool IsMenuOpened = true;
    }

    namespace Esp {
        bool Enabled = true;

        namespace Enemy {
            bool DrawEnemy = true;
            bool DrawBoxes = true;
            bool BoxOutline = true;
            int BoxType = 0;
            int BoxStyle = 1;
            float BoxThickness = 1.0;
        }


        namespace Teammate {
            bool DrawTeam = false;
            bool DrawBoxes = true;
            bool BoxOutline = true;
            int BoxType = 0;
            int BoxStyle = 0;
            float BoxThickness = 1.0;
        }
    }

    namespace Color {
        namespace Esp {
            namespace Enemy {
                float InvisibleBoxColor[4] = { 0.99, 0, 0, 0.99 };
                float VisibleBoxColor[4] = { 0, 0.99, 0, 0.99 };
                float KnockedBoxColor[4] = { 0.990, 0.671, 0.119, 0.99 };
                float InvisibleFilledBoxColor[4] = { 0, 0, 0, 0.11 };
                float VisibleFilledBoxColor[4] = { 0, 0, 0, 0.11 };
                float KnockedFilledBoxColor[4] = { 0, 0, 0, 0.11 };
            }

            namespace Teammate {
                float InvisibleBoxColor[4] = { 0.0846, 0.0693, 0.990, 0.99 };
                float VisibleBoxColor[4] = { 0.00990, 0.827, 0.990, 0.99 };
                float KnockedBoxColor[4] = { 0.484, 0, 0.990, 0.99 };
                float InvisibleFilledBoxColor[4] = { 0.0846, 0.0693, 0.990, 0.99 };
                float VisibleFilledBoxColor[4] = { 0.00990, 0.827, 0.990, 0.99 };
                float KnockedFilledBoxColor[4] = { 0.484, 0, 0.990, 0.99 };
            }
        }
    }
}