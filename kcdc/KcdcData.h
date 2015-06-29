// -----------------------------------------------------------------------
///
///  @file:   KcdcData.h
///
///  @author: Doug Reitz\n
///           https://github.com/dreitz/ \n
///
///  @date:   20-May-2015\n
///
///  @brief   Data processing and utilities for KASCADE Cosmic Ray Data Centre (KCDC) Project data
///           Utilities to process KASCADE Cosmic Ray Data Centre (KCDC) Project data.
///           https://kcdc.ikp.kit.edu/
///
///  @details Some methods are based on or translated from golang https://github.com/soniakeys/meeus.git by Sven Schoo.
///           Developed under the guidance of Dr. Robert Ehrlich http://mason.gmu.edu/~rehrlich/
///
///  Copyright The MIT License (MIT)
///
///            Copyright (c) 2015 Doug Reitz
///
///            Permission is hereby granted, free of charge, to any person obtaining a copy
///            of this software and associated documentation files (the "Software"), to deal
///            in the Software without restriction, including without limitation the rights
///            to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
///            copies of the Software, and to permit persons to whom the Software is
///            furnished to do so, subject to the following conditions:
///
///            The above copyright notice and this permission notice shall be included in
///            all copies or substantial portions of the Software.
///
///            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
///            IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
///            FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
///            AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
///            LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
///            OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
///            THE SOFTWARE.
///
//-------------------------------------------------------------------------
#ifndef _Csi_KcdcData_h_
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <valarray>
#include <inttypes.h>
#include <libnova/transform.h>
#include <stdlib.h>



namespace Csi
{
namespace Kcdc
{
namespace DataConstants
{
    const double KASCADE_LATITUDE = 49.0994;
    const double KASCADE_LONGITUDE = 8.4378;
    const double DEG2RAD = M_PI/180.0;
    const double RAD2DEG = 180.0/M_PI;
    const double PI_2 = M_PI*0.5;
    const double GAL_N_POLE_RA = 192.859508;
    const double GAL_N_POLE_DEC = 27.128336;
    const double GAL_LON0 = 122.932;
}

///  @brief   Data processing and class for KASCADE Cosmic Ray Data Centre (KCDC) datasets
///           https://kcdc.ikp.kit.edu/
class KcdcData
{
public:
    KcdcData() {}

    ///@brief Add fields to data input file
    ///
    ///This reads the KCDC input data and adds RA DEC LON LAT JDAYS DIST to the output file
    ///The input data fields need to be as follows\n
    ///         E          YC          XC          ZE          AZ          NE         NMU     ESUMHAD        NHAD           T           P          GT          MT         YMD         HMS           R          EV         AGE \n
    ///   15.0428     43.7400     79.5148     44.2007      7.4743      3.9608      3.8254     -1.0000          -1     20.2800   1001.3107   899391407   756894400    19980702      145647        1000       10007      1.1117 \n
    ///\n
    /// To get data in this format do the following  \n
    /// 1. Goto kcdc.ikp.kit.edu \n
    /// 2. Create an account by supplying the requested information. \n
    /// 3. Deselect (make green boxes white) the following 9 variables: electron number, hadron energy, air temperature, age, core position, muon number, hadron number, and air pressure. \n
    /// 4. Click submission & the data will download into a rather large file. \n
    /// \n
    ///@param ifname The input file name
    ///@param ofname The output file name
    ///@param maxDistance The output data is filtered to only include data with DIST <= to this value. If 0.0, no maximum is applied
    void AddFields(const std::string& ifname, const std::string& ofname, double maxDistance=0.0)
    {
        using namespace std;
        using namespace Kcdc::DataConstants;
        m_in.open(ifname.c_str(), std::ios::binary);
        m_out.open(ofname.c_str(), std::ios::binary);
        string s;
        getline(m_in, s);
        // output header line
        m_out.precision(4);
        m_out << fixed;
        m_out << s
                << setw(12) << right << "RA"
                << setw(12) << right << "DEC"
                << setw(12) << right << "LON"
                << setw(12) << right << "LAT"
                << setw(20) << right << "JDAYS"
                << setw(12) << right << "DIST"
                << "\n";
        uint64_t lines(0);
        std::cout << "\nProcessing input (" << ifname << ") output (" << ofname << ")";
        if (maxDistance>0.0) std::cout << " using max distance (" << maxDistance << ")";
        std::cout << "\n";
        //while (!m_in.eof() && !m_in.fail() && lines<100)
        while (!m_in.eof() && !m_in.fail())
        {
            ++lines;
            getline(m_in, s);
            if (m_in.eof()) break;
            std::istringstream iss(s);
            // existing
            double e(0.0), yc(0.0), xc(0.0), ze(0.0), az(0.0), ne(0.0), nmu(0.0),
                   esumhad(0.0);
            int64_t nhad(0);
            double t(0.0), p(0.0);
            uint64_t gt(0), mt(0), ymd(0), hms(0), r(0), ev(0);
            double age(0.0);
            // missing
            uint64_t mmn(0);
            // new
            double ra(0.0), dec(0.0), lon(0.0), lat(0.0), jdays(0.0), dist(0.0);

            iss>>e; iss>>yc; iss>>xc; iss>>ze; iss>>az; iss>>ne; iss>>nmu; iss>>esumhad; iss>>nhad; iss>>t; iss>>p;
            iss>>gt; iss>>mt; iss>>ymd; iss>>hms; iss>>r; iss>>ev; iss>>age;

            jdays = GetJulianDate(ymd, hms, mmn);    /// no mmn in data
            ln_hrz_posn inHrzPos;
            ln_lnlat_posn observerPos;
            ln_equ_posn equPos;
            inHrzPos.alt = 90.0 - ze;
            inHrzPos.az = EnsureCorrectRange(az + 180.0);
            observerPos.lat = KASCADE_LATITUDE;
            observerPos.lng = KASCADE_LONGITUDE;
            ln_get_equ_from_hrz(&inHrzPos, &observerPos, jdays, &equPos);
            ra = Convert360To180(equPos.ra);
            dec = equPos.dec;
            ln_gal_posn galacticPos;
            ln_get_gal_from_equ(&equPos, &galacticPos);
            lat = galacticPos.b;
            lon = galacticPos.l;

             //dist = sqrt(pow((dec-40.95),2.0) + pow((ra-308.0),2.0)/pow(cos(40.95*DEG2RAD),2.0));
            dist = sqrt(pow((dec-40.95),2.0) + pow((ra+52.0),2.0)/pow(cos(40.95*DEG2RAD),2.0));
            if (e>=15.0 && dist<=maxDistance)
            {
                m_out   //<< s
                        << setw(11) << right << e
                        << setw(12) << right << yc
                        << setw(12) << right << xc
                        << setw(12) << right << ze
                        << setw(12) << right << az
                        << setw(12) << right << ne
                        << setw(12) << right << nmu
                        << setw(12) << right << esumhad
                        << setw(12) << right << nhad
                        << setw(12) << right << t
                        << setw(12) << right << p
                        << setw(12) << right << gt
                        << setw(12) << right << mt
                        << setw(12) << right << ymd
                        << setw(12) << right << hms
                        << setw(12) << right << r
                        << setw(12) << right << ev
                        << setw(12) << right << age

                        << setw(13) << right << ra
                        << setw(12) << right << dec
                        << setw(12) << right << lon
                        << setw(12) << right << lat
                        << setw(20) << right << setprecision(6) <<  jdays << setprecision(4)
                        << setw(12) << right << dist
                        << "\n";
            }

            if ((lines%1000000)==0) std::cout << " " << lines <<" records processed\n";
            else if ((lines%50000)==0) std::cout << "." << std::flush;

        }
        m_in.close();
        m_out.close();
        std::cout << "\nComplete!\n";
    }

    struct EventInfo
    {
        EventInfo() : e(0.0), zenith(0.0), azimuth(0.0), jdate(0.0), dec(0.0), ra(0.0) {}
        EventInfo(const EventInfo& rhs) {e=rhs.e; zenith=rhs.zenith; azimuth=rhs.azimuth; jdate=rhs.jdate; dec=rhs.dec; ra=rhs.ra;}
        double e;
        double zenith;
        double azimuth;
        double jdate;
        double dec;
        double ra;
    };

    void ProcessEventStats(const std::string& ifname, const std::string& ofname, double emin, double emax )
    {
        using namespace std;
        using namespace Kcdc::DataConstants;
        static const double binSize = 0.5;
        static const uint64_t eventSetSize = 100000;
        srandom(13);
        m_in.open(ifname.c_str(), std::ios::binary);
        std::string tmp = ofname + std::string(".nreal.dat");
        std::ofstream nrealFile(tmp.c_str(), std::ios::binary);
        tmp = ofname + std::string(".nfake.dat");
        std::ofstream nfakeFile(tmp.c_str(), std::ios::binary);

        string s;
        getline(m_in, s);
        std::valarray<EventInfo> events(eventSetSize);
        class Array2D
        {
        public:
            Array2D(uint32_t rows, uint32_t cols, uint32_t val)
            : m_rows(rows), m_cols(cols), m_data(val, rows*cols) {}
            /// @brief sets all matrix elements to 0.0
            void zeroize() {m_data = 0.0;}
            /************* column major  *****************/
            uint64_t& operator()(uint i,uint j) {return m_data[i*m_cols+j];}
            /************** row major **************/
            //double& operator()(uint i,uint j) {return m_data[i*m_cols+j];}
        protected:
            std::valarray<uint64_t> m_data;
            uint32_t m_rows;
            uint32_t m_cols;
        };
        Array2D nreal(360, 720, 0);
        Array2D nfake(360, 720, 0);

        uint64_t lines(0);
        std::cout << "\nProcessing input (" << ifname << ") output (" << ofname << ")";
        std::cout << "\n";

        uint32_t eventCount(0);              // used to count events processed 0-100000;

        ln_lnlat_posn observerPos;
        observerPos.lat = KASCADE_LATITUDE;
        observerPos.lng = KASCADE_LONGITUDE;

        //while (!m_in.eof() && !m_in.fail() && lines<5000000)
        while (!m_in.eof() && !m_in.fail())
        {
            ++lines;
            getline(m_in, s);
            if (m_in.eof()) break;
            std::istringstream iss(s);
            // existing
            double e(0.0), yc(0.0), xc(0.0), ze(0.0), az(0.0), ne(0.0), nmu(0.0),
                   esumhad(0.0);
            int64_t nhad(0);
            double t(0.0), p(0.0);
            uint64_t gt(0), mt(0), ymd(0), hms(0), r(0), ev(0);
            double age(0.0);
            // missing
            uint64_t mmn(0);
            // new
            double ra(0.0), dec(0.0), lon(0.0), lat(0.0), jdays(0.0), dist(0.0);

            iss>>e; iss>>yc; iss>>xc; iss>>ze; iss>>az; iss>>ne; iss>>nmu; iss>>esumhad; iss>>nhad; iss>>t; iss>>p;
            iss>>gt; iss>>mt; iss>>ymd; iss>>hms; iss>>r; iss>>ev; iss>>age;

            jdays = GetJulianDate(ymd, hms, mmn);    /// no mmn in data
            ln_hrz_posn inHrzPos;

            ln_equ_posn equPos;
            inHrzPos.alt = 90.0 - ze;
            inHrzPos.az = EnsureCorrectRange(az + 180.0);

            ln_get_equ_from_hrz(&inHrzPos, &observerPos, jdays, &equPos);
            ra = Convert360To180(equPos.ra);
            dec = equPos.dec;
            ln_gal_posn galacticPos;
            ln_get_gal_from_equ(&equPos, &galacticPos);
            lat = galacticPos.b;
            lon = galacticPos.l;
             if (e>=emin && e<=emax)
            {
                //    minra  maxra   mindec   maxdec
                //     -180    180 -10.8794  89.9953

                EventInfo ei;
                ei.e = e;
                ei.zenith = inHrzPos.alt;
                ei.azimuth= inHrzPos.az;
                ei.jdate = jdays;
                ei.dec = dec + 90.0;   // shift to match nreal indexing bins
                ei.ra = ra + 180.0;    // shift to match nreal indexing bins

                uint32_t decIdx(ei.dec/binSize);
                uint32_t raIdx(ei.ra/binSize);
                if (ei.dec<0 || ei.dec>180.0) std::cout << "Illegal dec= " << ei.dec << "\n";
                if (ei.ra<0 || ei.ra>360.0) std::cout << "Illegal ra= " << ei.ra << "\n";
                if (decIdx>359 || raIdx>719)
                {
                    std::cout << "Illegal index " << decIdx << ":"<<raIdx << "\n";
                }

                ++nreal(decIdx, raIdx);
                events[eventCount] = ei; 
                ++eventCount;
                if (eventCount == eventSetSize)
                {
                    // generate 20 random events for each real event
                    // use zenith/azimuth of one real event
                    // use jdate from 20 other real events (selected randomly)
                    for (uint32_t n=0; n<eventSetSize; ++n)
                    {
                        EventInfo ei = events[n];
                        for (uint64_t i=0; i<20; ++i)
                        {
                            uint64_t idx((double)eventSetSize*((double)random()/(double)RAND_MAX));
                            if (idx>=eventSetSize)
                            {
                                std::cout << "illegal event idx :" << idx << "\n";
                            }
                            ei.jdate = events[idx].jdate;
                            ln_hrz_posn inHrzPos;
                            ln_equ_posn equPos;
                            inHrzPos.alt = ei.zenith;
                            inHrzPos.az = ei.azimuth;
                            ln_get_equ_from_hrz(&inHrzPos, &observerPos, ei.jdate, &equPos);
                            double fakeRa = Convert360To180(equPos.ra)+180.0;
                            double fakeDec = equPos.dec + 90.0;
                            if (fakeDec<0 || fakeDec>180.0) std::cout << "Illegal fakeDec= " << fakeDec << "\n";
                            if (fakeRa<0 || fakeRa>360.0) std::cout << "Illegal fakeRa= " << fakeRa << "\n";
                            uint32_t fakeDecIdx(fakeDec/binSize);
                            uint32_t fakeRaIdx(fakeRa/binSize);
                            if (fakeDecIdx>359 || fakeRaIdx>719)
                            {
                                std::cout << "Illegal index " << fakeDecIdx << ":"<<fakeRaIdx << "\n";
                            }
                            ++nfake(fakeDecIdx,fakeRaIdx);
                        }
                        eventCount = 0;
                    }
                }
            }

            if ((lines%1000000)==0) std::cout << " " << lines <<" records processed\n";
            else if ((lines%50000)==0) std::cout << "." << std::flush;

        }
        m_in.close();
        std::cout << "\nComplete!\n";
        std::cout << "Outputting matrices ...";
        // dec row, ra col
        // upper left dec=90, ra=-180
        for (int64_t i=359; i>=0; --i)
        {
            for(uint32_t j=0; j<720; ++j)
            {
                nrealFile << nreal(i,j) << " ";
                nfakeFile << (nfake(i,j)/20) << " ";
            }
            nrealFile << "\n";
            nfakeFile << "\n";
        }
        std::cout << "\nComplete!\n";
    }


    ///@brief Get Julian date from input
    ///
    /// Based on or translated from golang https://github.com/soniakeys/meeus.git
    double GetJulianDate(uint64_t ymd, uint64_t hms, uint64_t mmn)
    {
        using namespace std;

        double years = floor(ymd / 10000);
        double months = floor((ymd - years * 10000) / 100);
        double days = (ymd - years * 10000 - months *100);

        if (months <= 2) {
            months += 12;
            years--;
        }

        double hours = hms / 10000;
        double minutes = (hms - hours * 10000.0) / 100.0;
        double seconds = (double)(hms - hours * 10000.0 - minutes *100.0) + mmn*1e-9;

        days += (hours + (minutes + seconds / 60.0) / 60.0) / 24.0;
        double b = floor(years / 100.0);
        b = 2 - b + floor(b / 4);

        return floor(365.25 * floor(years + 4716.0)) + floor(306 * (months+1.0)/10.) + b + days - 1524.5;
    }

    ///@brief Get GST from Julian Days
    double GetGst(double julianDays)
    {
        double julian_day_part = 0.0;
        double day_fraction = modf(julianDays + 0.5, &julian_day_part);
        // (days - days_up_to_year_2000) / julian_days_per_century since J2000
        double julian_centuries = (julian_day_part - 2451545.5) / 36525.0;

        double coeffs[4] = {24110.54841, 8640184.812866, 0.093104, 0.0000062};
        double mean_gst0 = coeffs[3];
        int i = 3;
        while (i>0)
        {
            --i;
            mean_gst0 = mean_gst0*julian_centuries + coeffs[i];
        }
        return mean_gst0 + day_fraction*1.00273790935*86400.0;  // seconds
    }

    ///@brief Ensures input is in range 0-360
    double EnsureCorrectRange(double alpha)
    {
        double tmp = (int)(alpha / 360);
        if (alpha < 0.0) tmp --;
        tmp *= 360.0;
        return alpha - tmp;
    }

    ///@brief Convert from 360 degrees to +/-180
    double Convert360To180(double alpha)
    {
        if(alpha >= 180.)return alpha - 360.;
        return alpha;
    }

    ///@brief Apply Hammer projection to input ra and dec
    void ProjectHammerAitoff(double &ra, double &dec)
    {
        using namespace Kcdc::DataConstants;
        double z = sqrt(1.0+cos(dec*DEG2RAD)*cos(ra*DEG2RAD/2.0));
        ra = 180.0*cos(dec*DEG2RAD)*sin(ra*DEG2RAD/2.0)/z;
        dec = 90.0*sin(dec*DEG2RAD)/z;
    }

protected:
    std::ofstream m_out;
    std::ifstream m_in;

};

} // end namespace Kcdc
} // end namespace Csi
#endif // _Csi_KcdcData_h_



