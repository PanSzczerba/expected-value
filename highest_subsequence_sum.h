#pragma once
template <class Iter>
struct Span
{
    Iter start;
    Iter stop;
    Span(Iter start, Iter stop)
    {
        this->start = start;
        this->stop = stop;
    }

    Span(){}
};

template<typename Iter, typename Num = float>
Span<Iter> find_highest_subsequence_sum(Iter start_sequence, Iter stop_sequence)
{
   auto current_start = start_sequence;
   auto current_stop = start_sequence;

   Num current_sum = 0;
   auto max_start = start_sequence;
   auto max_stop = start_sequence;
   Num max_sum = *start_sequence;

   for(; current_stop != stop_sequence; current_stop++)
   {
       current_sum += *current_stop;

       if(current_sum > max_sum)
       {
           max_sum = current_sum;
           max_start = current_start;
           max_stop = current_stop;
       }

       if(current_sum <= 0)
       {
           current_start = current_stop + 1;
           current_sum = 0;
       }
   }

   return Span<Iter>(max_start, max_stop + 1);
}

