package com.example.tp1.fragments;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.ListFragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.ViewSwitcher;

import com.example.tp1.R;
import com.example.tp1.activities.DrawerMenuActivity;
import com.example.tp1.adapters.ForumPostAdapter;
import com.example.tp1.models.ForumPost;
import com.example.tp1.models.User;
import com.google.gson.Gson;

import java.util.ArrayList;
import java.util.List;


public class ForumFragment extends Fragment {
    private ListView listForumPostViews;
    private User currentUser;


    public static ForumFragment newInstance(){
        return (new ForumFragment());
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_forum, container, false);
        currentUser = new Gson().fromJson(getArguments().getString("user"), User.class);
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        listForumPostViews = (ListView)getView().findViewById(R.id.list_posts_forum);
        View headerView = getLayoutInflater().inflate(R.layout.fragment_forum_header_list, null);
        listForumPostViews.addHeaderView(headerView);
        ForumPostAdapter adapt = new ForumPostAdapter(load(),getActivity().getBaseContext());
        listForumPostViews.setAdapter(adapt);
    }


    private List<ForumPost> load() {
        List<ForumPost> result = new ArrayList();
        result.add(new ForumPost("titre1", "content1", "author1"));
        result.add(new ForumPost("titre2", "content2", "author2"));
        result.add(new ForumPost("titre3", "content3", "author3"));
        result.add(new ForumPost("titre4", "content4", "author4"));

        return result;
    }

}
