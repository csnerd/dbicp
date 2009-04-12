#include "dbicp.h"

#define INF 100000000

// Constants for Blackboard
#define WIDTH 1000
#define HEIGHT 1000
#define DEPTH 1
#define NB_CHANNELS 3

// Constants for gradient descent
#define RHO_0 1e-10
#define RHO_1 1e-10
#define EPSILON_0 1e-3
#define EPSILON_1 1e-3
#define GD_NITER_MAX 10

// Constant for Beaton Tukey
#define BT_A 1e7

// Constant for DBICP
#define DBICP_NITER_MAX 10

// Constant for saving
#define SAVE false

using namespace cimg_library;
using namespace std;


/*****************************************
*            CONSTRUCTOR                 *
******************************************/

DBICP::DBICP(PointSet ps1, PointSet ps2) {
    this->ps1=ps1;
    this->ps2=ps2;

    corres.resize(ps1.size());
    ps1_img.resize(ps1.size());
    ps2_NN2img.resize(ps1.size());
    box_mask.resize(ps1.size());

    error=0;
    transfo=Transfo();

/*
    // Initialize with mean translation instead of identity
    transfo.t11 = ps2.get_x_mean()-ps1.get_x_mean();
    transfo.t21 = ps2.get_y_mean()-ps1.get_y_mean();
*/

    Blackboard.assign(WIDTH,HEIGHT,DEPTH,NB_CHANNELS);

    box.assign(0,0,600,600);
    box.display();
    ps1.is_in_bounding_box(box,box_mask);

    cout << "Initial transformation:"<< endl;
    transfo.display();




}



/*****************************************
*            GENERAL ALGORITHM           *
******************************************/

void DBICP::perform() {
    cout << "Performing DBICP..." << endl<< endl;

    for (unsigned int i=0;i<DBICP_NITER_MAX;i++){
        perform_matching_step();
        perform_optim_step();
    }

    cout << "Estimated transformation:" << endl;
    transfo.display();

    // Update everything for ALL the points (not only the one in the bounding box)
    transfo(ps1,ps1_img);
    compute_corres(true);

    display_and_save();



}



/*****************************************
*           MATCHING FUNCTIONS           *
******************************************/
void DBICP::perform_matching_step(){
    // Updates images
    transfo(ps1,ps1_img,box_mask);
    // Compute correspondances
    compute_corres();

}

void DBICP::compute_corres(bool all_points) {
    /**
    * Compute correspondances (update corres and ps2_NN2img) for the point in the Bounding Box (except if all_points is true).
    * The L2 error is also stored.
    */

    error=0;

    for (unsigned int i=0;i<ps1.size();i++) {
        if (box_mask[i] or all_points) {
            double error_i_min = INF;

            for (unsigned int j=0;j<ps2.size();j++) {
                if (ps1_img[i].get_dist_with(ps2[j])<error_i_min) {
                    error_i_min = ps1_img[i].get_dist_with(ps2[j]);
                    corres[i]=j;
                    ps2_NN2img[i]=ps2[j];
                }
            }
            error+=error_i_min;
        }
    }

}



/*****************************************
*      OPTIMISATION FUNCTIONS            *
******************************************/

void DBICP::perform_optim_step() {
    // We use only similarity for now
    Similarity S=get_optimal_similarity();
    transfo=S;
}

Similarity DBICP::get_optimal_similarity() {
    return get_optimal_similarity_using_gd();
}

Similarity DBICP::get_optimal_similarity_using_gd() {
    /**
    * Get the optimal similarity using gradient descent.
    * It uses the cost function DBICP::cost
    * The number of iterations if GD_NITER_MAX
    */

    Similarity S(transfo.t11,transfo.t12,transfo.t13,transfo.t21);

    for (unsigned int i=0;i<GD_NITER_MAX;i++){
        S.t11 -= RHO_0*(cost(Similarity(S.t11+EPSILON_0,S.t12,S.t13,S.t21))-cost(S))/EPSILON_0;
        S.t12 -= RHO_1*(cost(Similarity(S.t11,S.t12+EPSILON_1,S.t13,S.t21))-cost(S))/EPSILON_1;
        S.t13 -= RHO_1*(cost(Similarity(S.t11,S.t12,S.t13+EPSILON_1,S.t21))-cost(S))/EPSILON_1;
        S.t21 -= RHO_0*(cost(Similarity(S.t11,S.t12,S.t13,S.t21+EPSILON_0))-cost(S))/EPSILON_0;

        S=Similarity(S.t11,S.t12,S.t13,S.t21);
    }

    return S;

}

double DBICP::cost(const Transfo &T) {
    /**
    * Cost function a given transformation, for the points in the Bounding Box
    */
    T(ps1,ps1_img,box_mask); // Just to be sure the image points are computed

    return Beaton_Tukey_rho(ps1_img.get_dist_with(ps2_NN2img,box_mask));
}

double DBICP::Beaton_Tukey_rho(const double &u) const {
    /**
    * Beaton Tukey function
    */
    if (abs(u)>BT_A){
        return BT_A*BT_A/6;
    } else {
        return BT_A*BT_A/6*(1-pow(1-pow(u/BT_A,2),3));
    }
}


/*****************************************
*        DISPLAY & SAVE FUNCTIONS        *
******************************************/


void DBICP::draw_corres(const unsigned char color1[],const unsigned char color2[]) {
    /**
    * This function draw arrows on the blackboard, from ps1 to ps1_img, and from ps1_img to ps2_NN2img.
    * Color1 within the bounding box, Color2 without
    */

    for (unsigned int i=0; i<ps1.size();i++){
        if (box_mask[i]) {
            Blackboard.draw_arrow(arrow_p(ps1[i].x),arrow_p(ps1[i].y),arrow_p(ps1_img[i].x),arrow_p(ps1_img[i].y),color1,30,10);
            Blackboard.draw_arrow(arrow_p(ps1_img[i].x),arrow_p(ps1_img[i].y),arrow_p(ps2_NN2img[i].x),arrow_p(ps2_NN2img[i].y),color1,30,10);
        }
        else {
            Blackboard.draw_arrow(arrow_p(ps1[i].x),arrow_p(ps1[i].y),arrow_p(ps1_img[i].x),arrow_p(ps1_img[i].y),color2,25,8);
            Blackboard.draw_arrow(arrow_p(ps1_img[i].x),arrow_p(ps1_img[i].y),arrow_p(ps2_NN2img[i].x),arrow_p(ps2_NN2img[i].y),color2,25,8);
        }
    }
}

void DBICP::display_and_save() {
    unsigned char COLOR_orange[]={248,90,4}, COLOR_blue[]={0,0,255}, COLOR_green[]={0,255,0}, COLOR_red[]={255,0,0}, COLOR_purple[]={140,7,131}, COLOR_dark_blue[]={0,0,130};

    ps1.draw_points(Blackboard,COLOR_green);
    ps2.draw_points(Blackboard,COLOR_red);
    ps1_img.draw_points(Blackboard,COLOR_orange);

    draw_corres(COLOR_blue,COLOR_dark_blue);

    box.draw(Blackboard,COLOR_purple);

    //Blackboard.draw_text("Basic ICP - Best Similarity",700,50,COLOR_purple);

    if (SAVE) {
        string filename="Output/Basic ICP - Best Similarity - "+to_string(DBICP_NITER_MAX)+" DBCIP iter - "+to_string(GD_NITER_MAX)+" GD iter - RHO_0 "+to_string(RHO_0)+" - RHO_1 "+to_string(RHO_1)+" - EPSILON_0 "+to_string(EPSILON_0)+" - EPSILON_1 "+to_string(EPSILON_1)+".bmp";
        Blackboard.save(filename.c_str());
    }

    Blackboard.display("Good job, right?");

}


