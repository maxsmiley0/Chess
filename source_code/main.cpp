//
//  main.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/20/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "game.h"

int main()
{
    while (true)
    {
        Timer t1;
        t1.start();
        Game g1("1rbq1rk1/p1b1nppp/1p2p3/8/1B1pN3/P2B4/1P3PPP/2RQ1R1K w - -", WHITE);
        Game g2("3r2k1/p2r1p1p/1p2p1p1/q4n2/3P4/PQ5P/1P1RNPP1/3R2K1 b - -", BLACK);
        Game g3("3r2k1/1p3ppp/2pq4/p1n5/P6P/1P6/1PB2QP1/1K2R3 w - -", WHITE);
        Game g4("r1b1r1k1/1ppn1p1p/3pnqp1/8/p1P1P3/5P2/PbNQNBPP/1R2RB1K w - -", BLACK);
        Game g5("2r4k/pB4bp/1p4p1/6q1/1P1n4/2N5/P4PPP/2R1Q1K1 b - -", WHITE);
        Game g6("r5k1/3n1ppp/1p6/3p1p2/3P1B2/r3P2P/PR3PP1/2R3K1 b - -", BLACK);
        Game g7("2r2rk1/1bqnbpp1/1p1ppn1p/pP6/N1P1P3/P2B1N1P/1B2QPP1/R2R2K1 b - -", WHITE);
        Game g8("5r1k/6pp/1n2Q3/4p3/8/7P/PP4PK/R1B1q3 b - -", BLACK);
        Game g9("r3k2r/pbn2ppp/8/1P1pP3/P1qP4/5B2/3Q1PPP/R3K2R w KQkq -", WHITE);
        Game g10("3r2k1/ppq2pp1/4p2p/3n3P/3N2P1/2P5/PP2QP2/K2R4 b - -", BLACK);
        Game g11("q3rn1k/2QR4/pp2pp2/8/P1P5/1P4N1/6n1/6K1 w - -", WHITE);
        Game g12("6k1/p3q2p/1nr3pB/8/3Q1P2/6P1/PP5P/3R2K1 b - -", BLACK);
        Game g13("1r4k1/7p/5np1/3p3n/8/2NB4/7P/3N1RK1 w - -", WHITE);
        Game g14("1r2r1k1/p4p1p/6pB/q7/8/3Q2P1/PbP2PKP/1R3R2 w - -", BLACK);
        Game g15("r2q1r1k/pb3p1p/2n1p2Q/5p2/8/3B2N1/PP3PPP/R3R1K1 w - -", WHITE);
        Game g16("8/4p3/p2p4/2pP4/2P1P3/1P4k1/1P1K4/8 w - -", BLACK);
        Game g17("1r1q1rk1/p1p2pbp/2pp1np1/6B1/4P3/2NQ4/PPP2PPP/3R1RK1 w", WHITE);
        Game g18("q4rk1/1n1Qbppp/2p5/1p2p3/1P2P3/2P4P/6P1/2B1NRK1 b - -", BLACK);
        Game g19("r2q1r1k/1b1nN2p/pp3pp1/8/Q7/PP5P/1BP2RPN/7K w - -", WHITE);
        Game g20("8/5p2/pk2p3/4P2p/2b1pP1P/P3P2B/8/7K w - -", BLACK);
        Game g21("8/2k5/4p3/1nb2p2/2K5/8/6B1/8 w - -", WHITE);
        Game g22("1B1b4/7K/1p6/1k6/8/8/8/8 w - -", BLACK);
        Game g23("rn1q1rk1/1b2bppp/1pn1p3/p2pP3/3P4/P2BBN1P/1P1N1PP1/R2Q1RK1 b - -", WHITE);
        Game g24("8/p1ppk1p1/2n2p2/8/4B3/2P1KPP1/1P5P/8 w - -", BLACK);
        Game g25("8/3nk3/3pp3/1B6/8/3PPP2/4K3/8 w - -", WHITE);
        Game g26("1rbq1rk1/p1b1nppp/1p2p3/8/1B1pN3/P2B4/1P3PPP/2RQ1R1K w - -", BLACK);
        Game g27("3r2k1/p2r1p1p/1p2p1p1/q4n2/3P4/PQ5P/1P1RNPP1/3R2K1 b - -", WHITE);
        Game g28("3r2k1/1p3ppp/2pq4/p1n5/P6P/1P6/1PB2QP1/1K2R3 w - -", BLACK);
        Game g29("r1b1r1k1/1ppn1p1p/3pnqp1/8/p1P1P3/5P2/PbNQNBPP/1R2RB1K w - -", WHITE);
        Game g30("2r4k/pB4bp/1p4p1/6q1/1P1n4/2N5/P4PPP/2R1Q1K1 b - -", BLACK);
        Game g31("r5k1/3n1ppp/1p6/3p1p2/3P1B2/r3P2P/PR3PP1/2R3K1 b - -", WHITE);
        Game g32("2r2rk1/1bqnbpp1/1p1ppn1p/pP6/N1P1P3/P2B1N1P/1B2QPP1/R2R2K1 b - -", BLACK);
        Game g33("5r1k/6pp/1n2Q3/4p3/8/7P/PP4PK/R1B1q3 b - -", WHITE);
        Game g34("r3k2r/pbn2ppp/8/1P1pP3/P1qP4/5B2/3Q1PPP/R3K2R w KQkq -", BLACK);
        Game g35("3r2k1/ppq2pp1/4p2p/3n3P/3N2P1/2P5/PP2QP2/K2R4 b - -", WHITE);
        Game g36("q3rn1k/2QR4/pp2pp2/8/P1P5/1P4N1/6n1/6K1 w - -", BLACK);
        Game g37("6k1/p3q2p/1nr3pB/8/3Q1P2/6P1/PP5P/3R2K1 b - -", WHITE);
        Game g38("1r4k1/7p/5np1/3p3n/8/2NB4/7P/3N1RK1 w - -", BLACK);
        Game g39("1r2r1k1/p4p1p/6pB/q7/8/3Q2P1/PbP2PKP/1R3R2 w - -", WHITE);
        Game g40("r2q1r1k/pb3p1p/2n1p2Q/5p2/8/3B2N1/PP3PPP/R3R1K1 w - -", BLACK);
        Game g41("8/4p3/p2p4/2pP4/2P1P3/1P4k1/1P1K4/8 w - -", WHITE);
        Game g42("1r1q1rk1/p1p2pbp/2pp1np1/6B1/4P3/2NQ4/PPP2PPP/3R1RK1 w", BLACK);
        Game g43("q4rk1/1n1Qbppp/2p5/1p2p3/1P2P3/2P4P/6P1/2B1NRK1 b - -", WHITE);
        Game g44("r2q1r1k/1b1nN2p/pp3pp1/8/Q7/PP5P/1BP2RPN/7K w - -", BLACK);
        Game g45("8/5p2/pk2p3/4P2p/2b1pP1P/P3P2B/8/7K w - -", WHITE);
        Game g46("8/2k5/4p3/1nb2p2/2K5/8/6B1/8 w - -", BLACK);
        Game g47("1B1b4/7K/1p6/1k6/8/8/8/8 w - -", WHITE);
        Game g48("rn1q1rk1/1b2bppp/1pn1p3/p2pP3/3P4/P2BBN1P/1P1N1PP1/R2Q1RK1 b - -", BLACK);
        Game g49("8/p1ppk1p1/2n2p2/8/4B3/2P1KPP1/1P5P/8 w - -", WHITE);
        Game g50("8/3nk3/3pp3/1B6/8/3PPP2/4K3/8 w - -", BLACK);
        
        std::ofstream file;

        int score = 0;

        score += g1.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g2.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g3.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g4.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g5.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g6.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g7.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g8.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g9.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g10.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g11.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g12.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g13.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g14.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g15.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g16.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g17.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g18.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g19.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g20.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g21.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g22.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g23.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g24.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g25.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g26.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g27.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g28.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g29.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g30.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g31.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g32.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g33.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g34.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g35.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g36.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g37.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g38.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g39.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g40.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g41.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g42.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g43.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g44.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g45.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g46.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g47.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g48.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g49.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
        score += g50.play();
        file.open("../../results.txt", std::ofstream::out | std::ofstream::app);
        //Error checking pointless as it will simply create the file
        file << score << std::endl;
        
        file << "Elapsed: " << t1.elapsed() <<std::endl;
        
        file.close();
    }
}