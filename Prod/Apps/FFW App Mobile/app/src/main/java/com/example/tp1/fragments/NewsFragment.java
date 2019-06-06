package com.example.tp1.fragments;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;

import com.example.tp1.R;
import com.example.tp1.adapters.NewsPostAdapter;
import com.example.tp1.models.NewsPost;
import com.example.tp1.models.User;
import com.google.gson.Gson;

import java.util.ArrayList;
import java.util.List;

public class NewsFragment extends Fragment {
    private ListView listNewsPostViews;
    private User currentUser;


    public static NewsFragment newInstance(){
        return (new NewsFragment());
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_news, container, false);
        currentUser = new Gson().fromJson(getArguments().getString("user"), User.class);
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        listNewsPostViews = (ListView)getView().findViewById(R.id.list_posts_news);
        View headerView = getLayoutInflater().inflate(R.layout.fragment_news_header_list, null);
        listNewsPostViews.addHeaderView(headerView);
        NewsPostAdapter adapt = new NewsPostAdapter(load(),getActivity().getBaseContext());
        listNewsPostViews.setAdapter(adapt);
    }


    private List<NewsPost> load() {
        List<NewsPost> result = new ArrayList();
        result.add(new NewsPost("titre1", "content1", "author1"));
        result.add(new NewsPost("titre2", "content2", "author2"));
        result.add(new NewsPost("titre3", "content3", "author3"));
        result.add(new NewsPost("titre4", "content4", "author4"));

        return result;
    }

}
