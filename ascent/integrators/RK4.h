// Copyright (c) 2016-2017 Anyar, Inc.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//      http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

// Fourth order, four pass Runge Kutta.

namespace asc
{
   template <typename state_t>
   struct RK4T
   {
      using value_t = typename state_t::value_type;

      template <typename System>
      void operator()(System&& system, state_t& x, value_t& t, const value_t dt)
      {
         const value_t t0 = t;
         const value_t dt_2 = half*dt;
         const value_t dt_6 = sixth*dt;

         const size_t n = x.size();
         if (xd.size() < n)
         {
            xd.resize(n);
            xd_temp.resize(n);
         }

         x0 = x;
         system(x, xd, t);
         for (size_t i = 0; i < n; ++i)
            x[i] = dt_2 * xd[i] + x0[i];
         t += dt_2;

         system(x, xd_temp, t);
         for (size_t i = 0; i < n; ++i)
         {
            xd[i] += two * xd_temp[i];
            x[i] = dt_2 * xd_temp[i] + x0[i];
         }

         system(x, xd_temp, t);
         for (size_t i = 0; i < n; ++i)
         {
            xd[i] += two * xd_temp[i];
            x[i] = dt * xd_temp[i] + x0[i];
         }
         t = t0 + dt;

         system(x, xd_temp, t);
         for (size_t i = 0; i < n; ++i)
            x[i] = dt_6 * (xd[i] + xd_temp[i]) + x0[i];
      }

   private:
      static constexpr auto two = static_cast<value_t>(2.0);
      static constexpr auto half = static_cast<value_t>(0.5);
      static constexpr auto sixth = static_cast<value_t>(1.0 / 6.0);
      state_t x0, xd, xd_temp;
   };
}