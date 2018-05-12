using Gallery.App.Commands;
using Gallery.BL;
using Gallery.BL.Messages;
using Gallery.BL.Models;
using Gallery.BL.Repository;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;

namespace Gallery.App.ViewModels
{
    public class PersonListViewModel : ViewModelBase
    {
        private readonly GalleryRepository galleryRepository;
        private readonly IMessenger messenger;
        private PhotographyDetailModel model;
        public ObservableCollection<PersonListModel> Persons { get; set; } = new ObservableCollection<PersonListModel>();
        public ICommand SelectPersonCommand { get; }
        public ICommand ListPersonCommand { get; set; }

        private bool selection_for_chosen_person;


        public PersonListViewModel(GalleryRepository galleryRepository, IMessenger messenger)
        {
            selection_for_chosen_person = false;
            this.galleryRepository = galleryRepository;
            this.messenger = messenger;
            SelectPersonCommand = new RelayCommands(PersonSelectionChanged);
            ListPersonCommand = new RelayCommands(PersonList);
            this.messenger.Register<PersonSearchMessage>(PersonSearch);
            this.messenger.Register<UpdatePersonMessage>(UpdatePersonFromList);

        }

        public void OnLoad()
        {
            if (selection_for_chosen_person == false)
            {
                messenger.Send(new HideDetailMessage());
            }
            Persons.Clear();
            var persons = galleryRepository.GetAllPerson();
            foreach (var person in persons)
            {
                Persons.Add(person);
            }
        }
        private void PersonSelectionChanged(object parameter)
        {
            if (parameter is PersonListModel person)
            {
                if (selection_for_chosen_person == false)
                {
                    messenger.Send(new SelectedPersonMessage { Id = person.Id });
                    messenger.Send(new UpdatePhotoMessage(null));
                    messenger.Send(new ChangeVisibilityMessage(true));

                }
                else if (selection_for_chosen_person == true)
                {
                    selection_for_chosen_person = false;
                    this.messenger.Send(new ChangeTabItemMessage(0));




                }

            }
        }
        private void PersonList()
        {
            OnLoad();
        }

        private void UpdatePersonFromList(UpdatePersonMessage messenger)
        {
            OnLoad();
        }

        private void PersonSearch(PersonSearchMessage messenger)
        {
            Persons.Clear();
            var persons = galleryRepository.GetAllPerson();
            foreach (var person in persons)
            {
                if ((galleryRepository.GetPersonById(person.Id).FirstName == messenger.Name) || (galleryRepository.GetPersonById(person.Id).LastName == messenger.Name))
                    Persons.Add(person);
            }
        }

    }

}